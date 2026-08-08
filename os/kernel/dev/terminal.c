#include "drivers.h"

// ============================================================
// 软件端终端模拟器 (内核侧)
// ttyd 只转发字符流, 本模块负责解析转义序列、维护主/备两块屏幕缓冲,
// 并用 SCREEN_CACHE1 + 脏行位图 + ctrl 协议驱动 screen 设备。
// 屏幕状态完全属于本模块, 进程退出路径不参与任何屏幕操作;
// 全屏程序通过 \e[?1049h / \e[?1049l 切换主/备缓冲, 主屏内容原样保留。
// ============================================================

#define TERM_MAX_W 512
#define TERM_MAX_H 200

static char term_main[TERM_MAX_W * TERM_MAX_H];   // 主缓冲 (shell 界面)
static char term_alt[TERM_MAX_W * TERM_MAX_H];    // 备用缓冲 (editor 等全屏程序)
static char* term_active = term_main;
static uint32_t term_w = 80, term_h = 24;
static uint32_t term_row = 1, term_col = 1;       // 光标 (1-based)
static uint32_t term_saved_row = 1, term_saved_col = 1;
static uint32_t term_prev_row = 0, term_prev_col = 0;
static char term_dirty[TERM_MAX_H];
static int term_state = 0;       // 0=文本 1=ESC 2=CSI
static char term_csi[32];
static int term_csi_n = 0;
static int term_csi_priv = 0;
static int term_full = 1;        // 整屏切换后需要全量重绘
static int term_inited = 0;

static void term_fill(char* dst, char ch, int n){
    for(int i = 0; i < n; i++) dst[i] = ch;
}

// 活动缓冲整体上滚一行, 最后一行清空, 全部行标记脏
static void term_scroll_up(void){
    for(uint32_t r = 0; r < term_h - 1; r++){
        for(uint32_t c = 0; c < term_w; c++)
            term_active[r*term_w + c] = term_active[(r+1)*term_w + c];
    }
    term_fill(term_active + (term_h-1)*term_w, ' ', term_w);
    for(uint32_t r = 0; r < term_h; r++) term_dirty[r] = 1;
}

static void term_clear(void){
    term_fill(term_active, ' ', term_w * term_h);
    for(uint32_t r = 0; r < term_h; r++) term_dirty[r] = 1;
}

// 普通字符写入活动缓冲
static void term_putc(char ch){
    switch(ch){
        case '\n':   // 与旧 tty 语义一致: 换行同时回到行首
            if(term_row < term_h) term_row++; else term_scroll_up();
            term_col = 1;
            break;
        case '\r':
            term_col = 1;
            break;
        case '\b':
            if(term_col > 1) term_col--;
            break;
        case '\t':
            term_col = ((term_col + 7) / 8) * 8 + 1;
            if(term_col > term_w) term_col = term_w;
            break;
        case 0:
            break;
        default:
            if(term_row >= 1 && term_row <= term_h && term_col >= 1 && term_col <= term_w){
                term_active[(term_row-1)*term_w + (term_col-1)] = ch;
                term_dirty[term_row-1] = 1;
            }
            term_col++;
            if(term_col > term_w){
                term_col = 1;
                if(term_row < term_h) term_row++; else term_scroll_up();
            }
            break;
    }
}

// 解析 CSI 参数: 取第 idx 个 ';' 分隔参数, 缺省为 def
static int term_csi_param(int idx, int def){
    int cur = 0, val = 0, seen = 0;
    for(int i = 0; i < term_csi_n; i++){
        char c = term_csi[i];
        if(c == ';'){
            if(cur == idx) return seen ? val : def;
            cur++; val = 0; seen = 0;
        } else if(c >= '0' && c <= '9'){
            val = val * 10 + (c - '0');
            seen = 1;
        }
    }
    return (cur == idx) ? (seen ? val : def) : def;
}

static void term_csi_exec(char final_ch){
    if(term_csi_priv){
        int p0 = term_csi_param(0, 1);
        if(p0 == 1049){                     // ?1049: 主/备缓冲切换
            if(final_ch == 'h'){
                term_active = term_alt;     // 主缓冲内容保留
                term_saved_row = term_row;
                term_saved_col = term_col;
                term_row = 1; term_col = 1;
                term_clear();
                term_full = 1;
            } else if(final_ch == 'l'){
                term_active = term_main;    // 切回主缓冲, 光标恢复
                term_row = term_saved_row;
                term_col = term_saved_col;
                term_full = 1;
            }
        }
        return;
    }
    int p0 = term_csi_param(0, 1);
    int p1 = term_csi_param(1, 1);
    switch(final_ch){
        case 'H': {                         // CUP: 光标定位
            if(term_csi_n == 0){ term_row = 1; term_col = 1; }
            else { term_row = p0; term_col = p1; }
            if(term_row < 1) term_row = 1;
            if(term_row > term_h) term_row = term_h;
            if(term_col < 1) term_col = 1;
            if(term_col > term_w) term_col = term_w;
            break;
        }
        case 'J':                           // ED: 清屏
            if(p0 == 2) term_clear();
            break;
        case 'K': {                         // EL: 清到行尾
            if(term_row >= 1 && term_row <= term_h){
                for(uint32_t c = term_col - 1; c < term_w; c++)
                    term_active[(term_row-1)*term_w + c] = ' ';
                term_dirty[term_row-1] = 1;
            }
            break;
        }
        default:
            break;
    }
}

// 字符流状态机
static void term_feed(char ch){
    if(term_state == 0){
        if(ch == 0x1b) term_state = 1;
        else term_putc(ch);
    } else if(term_state == 1){
        if(ch == '['){ term_state = 2; term_csi_n = 0; term_csi_priv = 0; }
        else term_state = 0;
    } else {
        if(ch >= 0x40 && ch <= 0x7e){
            term_csi_exec(ch);
            term_state = 0;
        } else if(ch == '?'){
            term_csi_priv = 1;
        } else if(term_csi_n < 31){
            term_csi[term_csi_n++] = ch;
        }
    }
}

// 把活动缓冲的脏行/光标同步到 screen 设备
static void term_paint(void){
    uint32_t w = *(uint32_t*)SCREEN_WIDTH_ADDR;
    uint32_t h = *(uint32_t*)SCREEN_HEIGHT_ADDR;
    if(w < 10) w = 10; if(w > TERM_MAX_W) w = TERM_MAX_W;
    if(h < 5)  h = 5;  if(h > TERM_MAX_H) h = TERM_MAX_H;
    if(w != term_w || h != term_h){
        // 终端尺寸变化: 重建为空白并整屏重绘
        term_w = w; term_h = h;
        term_fill(term_main, ' ', TERM_MAX_W * TERM_MAX_H);
        term_fill(term_alt,  ' ', TERM_MAX_W * TERM_MAX_H);
        term_active = term_main;
        if(term_row > term_h) term_row = term_h;
        if(term_col > term_w) term_col = term_w;
        term_full = 1;
    }
    if(term_full){
        uint32_t n = term_w * term_h;
        for(uint32_t i = 0; i < n; i++)
            *(char*)(SCREEN_CACHE1_ADDR + i) = term_active[i];
        for(uint32_t r = 0; r < term_h; r++)
            *(char*)(SCREEN_DIRTY_ADDR + r) = 1;
        *(uint32_t*)SCREEN_FRAME_ROW = term_row;
        *(uint32_t*)SCREEN_FRAME_COL = term_col;
        *(char*)SCREEN_CTRL_ADDR = 3;       // 整屏重绘
        term_prev_row = term_row; term_prev_col = term_col;
        term_full = 0;
        for(uint32_t r = 0; r < term_h; r++) term_dirty[r] = 0;
        return;
    }
    int any = 0;
    for(uint32_t r = 0; r < term_h; r++){
        if(!term_dirty[r]) continue;
        any = 1;
        char* dst = (char*)SCREEN_CACHE1_ADDR + r * term_w;
        for(uint32_t c = 0; c < term_w; c++)
            dst[c] = term_active[r * term_w + c];
        *(char*)(SCREEN_DIRTY_ADDR + r) = 1;
        term_dirty[r] = 0;
    }
    int cur_changed = (term_row != term_prev_row || term_col != term_prev_col);
    *(uint32_t*)SCREEN_FRAME_ROW = term_row;
    *(uint32_t*)SCREEN_FRAME_COL = term_col;
    // 普通输出走 ctrl=1: 宿主按 SCREEN_RENDER_INTERVAL 合并刷新,
    // 避免 ls 这类多次小 write 每次都立即整屏 flush, 造成行与行之间
    // 出现可感知的时间间隔; 编辑器整帧/需要立即刷新的场景才用 ctrl=2。
    if(any || cur_changed) *(char*)SCREEN_CTRL_ADDR = 1;
    term_prev_row = term_row; term_prev_col = term_col;
}

// ttyd 把输出字节流整体交给终端模拟器
void terminal_write(const char* buf, uint32_t count){
    if(!term_inited){
        term_fill(term_main, ' ', TERM_MAX_W * TERM_MAX_H);
        term_fill(term_alt,  ' ', TERM_MAX_W * TERM_MAX_H);
        term_active = term_main;
        term_inited = 1;
    }
    for(uint32_t i = 0; i < count; i++) term_feed(buf[i]);
    term_paint();
}
