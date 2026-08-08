#include "screen.h"
#include <cstdio>
#include <cstring>
#include <csignal>
#include <sys/ioctl.h>
#include <unistd.h>

// 宿主终端 resize 时内核发 SIGWINCH, 事件驱动刷新尺寸, 避免频繁 ioctl
static volatile sig_atomic_t g_winch = 0;
static void on_winch(int){ g_winch = 1; }

screen::screen(uint32_t size):vmem(size){
    signal(SIGWINCH, on_winch);
    first_render = true;
    memset(dirty, 0, sizeof(dirty));
    last_out_tick = 0;
    last_row = 0; last_col = 0;
    query_size();
}

screen::~screen(){
}

void screen::query_size(void){
    uint32_t w = 80, h = 24;
    struct winsize ws;
    if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_col > 0 && ws.ws_row > 0){
        w = ws.ws_col; h = ws.ws_row;
    }
    if(w > SCREEN_MAX_W) w = SCREEN_MAX_W;
    if(h > SCREEN_MAX_H) h = SCREEN_MAX_H;
    if(w < 10) w = 10;
    if(h < 5)  h = 5;
    this->put4B(0, w);
    this->put4B(4, h);
}

// 把数字写入输出缓冲
static void put_num(char* out, int* n, uint32_t v) {
    char tmp[12];
    int t = 0;
    if (v == 0) tmp[t++] = '0';
    while (v) { tmp[t++] = '0' + (v % 10); v /= 10; }
    while (t > 0) out[(*n)++] = tmp[--t];
}

// 输出所有脏行 + 光标, 清脏标记 (首次渲染整屏清屏重绘)
void screen::flush_dirty(uint32_t tick){
    uint32_t w = this->get4B(0);
    uint32_t h = this->get4B(4);
    if(w > SCREEN_MAX_W) w = SCREEN_MAX_W;
    if(h > SCREEN_MAX_H) h = SCREEN_MAX_H;
    uint32_t row = this->get4B(8);
    uint32_t col = this->get4B(12);
    if(row < 1) row = 1;
    if(row > h) row = h;
    if(col < 1) col = 1;
    if(col > w) col = w;
    last_row = row; last_col = col;
    char* out = new char[h * (w + 16) + 32];
    int n = 0;
    if(first_render){
        out[n++]='\033'; out[n++]='['; out[n++]='H';
        out[n++]='\033'; out[n++]='['; out[n++]='2'; out[n++]='J';
        for(uint32_t r=0;r<h;r++){
            out[n++]='\033'; out[n++]='[';
            put_num(out,&n,r+1);
            out[n++]=';'; out[n++]='1'; out[n++]='H';
            for(uint32_t c=0;c<w;c++){
                char ch=this->getB(20+r*w+c);
                if(!ch) ch=' ';
                out[n++]=ch;
            }
            dirty[r]=false;
        }
        first_render=false;
    }else{
        for(uint32_t r=0;r<h;r++){
            if(!dirty[r]) continue;
            out[n++]='\033'; out[n++]='[';
            put_num(out,&n,r+1);
            out[n++]=';'; out[n++]='1'; out[n++]='H';
            for(uint32_t c=0;c<w;c++){
                char ch=this->getB(20+r*w+c);
                if(!ch) ch=' ';
                out[n++]=ch;
            }
            dirty[r]=false;
        }
    }
    // 光标定位
    out[n++]='\033'; out[n++]='[';
    put_num(out,&n,row);
    out[n++]=';';
    put_num(out,&n,col);
    out[n++]='H';
    fwrite(out,1,n,stdout);
    fflush(stdout);
    delete[] out;
    last_out_tick = tick;
}

int screen::process(Bus* bus,uint32_t tick){
    if(g_winch){
        g_winch = 0;
        query_size();
        first_render = true;
        memset(dirty, 0, sizeof(dirty));
    }
    uint32_t w = this->get4B(0);
    uint32_t h = this->get4B(4);
    if(w < 10 || w > SCREEN_MAX_W || h < 5 || h > SCREEN_MAX_H){
        query_size();
        w = this->get4B(0);
        h = this->get4B(4);
    }
    uint32_t ctrl = this->getB(16);
    uint32_t row = this->get4B(8);
    uint32_t col = this->get4B(12);
    bool cur_changed = (row != last_row || col != last_col);
    if(ctrl==3){
        // 强制整屏重绘 (控制台恢复): 忽略脏行位图, 全量输出
        for(uint32_t r=0;r<h;r++) dirty[r] = true;
        flush_dirty(tick);
        this->putB(16,0);
        return 0;
    }
    if(ctrl==1 || ctrl==2){
        // 读内核写的脏行位图 (设备偏移 0x10004), 只吸收标记的行
        bool any = false;
        for(uint32_t r=0;r<h;r++){
            if(this->getB(0x10004 + r)){
                any = true;
                dirty[r] = true;
                this->putB(0x10004 + r, 0);
            }
        }
        // ctrl=2 (编辑器整帧): 立即输出, 即使无脏行也要刷新光标定位;
        // ctrl=1 (普通输出): 按间隔合并 (有脏行或光标移动都会触发)
        if(ctrl==2 || ((any || cur_changed) && (first_render || (int)(tick - last_out_tick) >= SCREEN_RENDER_INTERVAL))){
            flush_dirty(tick);
        }
        this->putB(16,0);
    }
    // 周期 flush 普通输出累积的脏行 (即使没有新 ctrl 请求)
    if((int)(tick - last_out_tick) >= SCREEN_RENDER_INTERVAL){
        bool any = false;
        for(uint32_t r=0;r<h && !any;r++) if(dirty[r]) any = true;
        if(any || cur_changed) flush_dirty(tick);
    }
    return 0;
}

// 关机前强制输出剩余脏行 (合并渲染模式下, 退出瞬间可能还没到下一次
// 周期 flush, 最后一帧会滞留; 这里在设备关闭前补一次)
void screen::flush_all(void){
    bool any = false;
    for(uint32_t r=0;r<SCREEN_MAX_H;r++) if(dirty[r]) { any = true; break; }
    if(any) flush_dirty(last_out_tick);
}
