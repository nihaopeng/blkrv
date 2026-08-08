// BLKRv 轻量文本编辑器 (精简版)
// 用法: editor <绝对路径>      (无参数时以 untitled 新建)
//
// 普通模式:
//   h/j/k/l 或方向键  移动光标
//   w / b / e         按单词移动 (前进/后退/词尾)
//   0 / ^ / $         行首 / 行首非空字符 / 行尾
//   gg / G            首行 / 末行
//   i / a / I / A     当前处 / 后一位 / 行首 / 行尾插入
//   o / O             下方 / 上方新建行并插入
//   x / X             删除光标处 / 光标前字符
//   / 模式            搜索 (Enter 查找)
//   : 模式            w / wq / q / q! / e <path> / w <path> / 行号
//   PgUp / PgDn       上 / 下翻页
//
// 插入模式:
//   输入字符, Enter 换行, Backspace 删除, Tab 插入 4 空格
//   Esc 返回普通模式, 方向键 / Home / End / Delete 可用

#define MAX_LINES 200
#define MAX_COL 96
#define MAX_TEXT (MAX_LINES * (MAX_COL + 1))
#define PROMPT_LEN 64

// 屏幕尺寸运行时从终端查询, 窗口变化后自适应
static int page_w = 80;
static int page_h = 24;

// 模式
#define MODE_NORMAL 0
#define MODE_INSERT 1
#define MODE_CMD    2
#define MODE_SEARCH 3

// 特殊按键 (ANSI 转义序列解析后返回负值)
#define KEY_UP     -1
#define KEY_DOWN   -2
#define KEY_LEFT   -3
#define KEY_RIGHT  -4
#define KEY_HOME   -5
#define KEY_END    -6
#define KEY_PGUP   -7
#define KEY_PGDN   -8
#define KEY_DEL    -9

static char filename[128] = "untitled";
static char lines[MAX_LINES][MAX_COL + 1];
static int nlines = 1;
static int cur_line = 0, cur_col = 0;
static int top = 0;         // 屏幕顶部行
static int col_off = 0;     // 水平滚动偏移
static int mode = MODE_NORMAL;
static int dirty = 0;
static int quit = 0;
static char msg[512];

// 命令 / 搜索输入
static char cmd_buf[PROMPT_LEN + 1];
static int cmd_len = 0;
static char pat_buf[PROMPT_LEN + 1];
static int pat_len = 0;
static char last_pat[PROMPT_LEN + 1];

// ---------------- syscalls ----------------
static int writef(int fd, const char* buf, int count) {
    int r;
    __asm__ volatile("li a7, 2\n mv a0, %1\n mv a1, %2\n mv a2, %3\n ecall\n mv %0, a0"
        : "=r"(r) : "r"(fd), "r"(buf), "r"(count) : "a0","a1","a2","a7");
    return r;
}
static int readf(int fd, void* buf, int count) {
    int r;
    __asm__ volatile("li a7, 3\n mv a0, %1\n mv a1, %2\n mv a2, %3\n ecall\n mv %0, a0"
        : "=r"(r) : "r"(fd), "r"(buf), "r"(count) : "a0","a1","a2","a7");
    return r;
}
static int openf(const char* path) {
    int r;
    __asm__ volatile("li a7, 0\n mv a0, %1\n ecall\n mv %0, a0" : "=r"(r) : "r"(path) : "a0","a7");
    return r;
}
static int createf(const char* path, int type, void* ino) {
    int r;
    __asm__ volatile("li a7, 4\n mv a0, %1\n mv a1, %2\n mv a2, %3\n ecall\n mv %0, a0"
        : "=r"(r) : "r"(path), "r"(type), "r"(ino) : "a0","a1","a2","a7");
    return r;
}
static int closef(int fd) {
    int r;
    __asm__ volatile("li a7, 25\n mv a0, %1\n ecall\n mv %0, a0" : "=r"(r) : "r"(fd) : "a0","a7");
    return r;
}
static int finfo(int fd, void* ino) {
    int r;
    __asm__ volatile("li a7, 20\n mv a0, %1\n mv a1, %2\n ecall\n mv %0, a0"
        : "=r"(r) : "r"(fd), "r"(ino) : "a0","a1","a7");
    return r;
}
static void exit_proc(void) {
    __asm__ volatile("li a7, 9\n ecall\n");
}
static int tty_size_sys(unsigned int* w, unsigned int* h) {
    int r;
    __asm__ volatile("li a7, 30\n mv a0, %1\n mv a1, %2\n ecall\n mv %0, a0"
        : "=r"(r) : "r"(w), "r"(h) : "a0","a1","a7");
    return r;
}
static void flush_input_sys(void) {
    // 丢弃启动前积压的输入, 避免提前输入被编辑器当成按键吃掉
    __asm__ volatile("li a7, 31\n ecall\n");
}

// ---------------- 终端输出 (ANSI 转义序列) ----------------
// 宿主侧 terminal 模拟器负责解析这些序列并渲染: 本编辑器只在进入时
// 发送 \e[?1049h 启用备用缓冲, 退出时发送 \e[?1049l 切回主缓冲,
// shell 的主屏内容由终端模拟器原样保留。
static void tput(const char* s) {
    int n = 0; while (s[n]) n++;
    writef(1, s, n);
}
static void tput_num(char* buf, int* n, int v) {
    char tmp[12]; int t = 0;
    if (v == 0) tmp[t++] = '0';
    while (v) { tmp[t++] = '0' + (v % 10); v /= 10; }
    for (int i = t - 1; i >= 0; i--) buf[(*n)++] = tmp[i];
}
static void tput_pos(int r, int c) {
    char buf[24]; int n = 0;
    buf[n++] = 27; buf[n++] = '[';
    tput_num(buf, &n, r);
    buf[n++] = ';';
    tput_num(buf, &n, c);
    buf[n++] = 'H';
    writef(1, buf, n);
}

// ---------------- 字符串 / 行操作 ----------------
static int slen(const char* s) { int n = 0; while (s[n]) n++; return n; }
static int scmp(const char* a, const char* b) {
    int la = slen(a), lb = slen(b);
    if (la != lb) return 0;
    for (int i = 0; i < la; i++) if (a[i] != b[i]) return 0;
    return 1;
}
static void scpy(char* d, const char* s) {
    int i = 0; while (s[i]) { d[i] = s[i]; i++; } d[i] = 0;
}
// 有界拷贝: 防止长路径溢出目标缓冲区
static void scpy_n(char* d, const char* s, int max) {
    int i = 0;
    while (s[i] && i < max - 1) { d[i] = s[i]; i++; }
    d[i] = 0;
}
static int line_len(int n) { return slen(lines[n]); }

static void ins_char(int ln, int col, char ch) {
    int len = line_len(ln);
    if (len >= MAX_COL) return;
    for (int i = len; i > col; i--) lines[ln][i] = lines[ln][i - 1];
    lines[ln][col] = ch;
    lines[ln][len + 1] = 0;
}
static void del_char(int ln, int col) {
    int len = line_len(ln);
    if (col >= len) return;
    for (int i = col; i < len; i++) lines[ln][i] = lines[ln][i + 1];
}
static void newline_after(int ln) {
    if (nlines >= MAX_LINES) return;
    for (int i = nlines; i > ln + 1; i--) scpy(lines[i], lines[i - 1]);
    lines[ln + 1][0] = 0;
    nlines++;
}
static void newline_before(int ln) {
    if (nlines >= MAX_LINES) return;
    for (int i = nlines; i > ln; i--) scpy(lines[i], lines[i - 1]);
    lines[ln][0] = 0;
    nlines++;
}
static void split_line(int ln, int col) {
    if (nlines >= MAX_LINES) return;
    for (int i = nlines; i > ln + 1; i--) scpy(lines[i], lines[i - 1]);
    int j = 0;
    for (int k = col; lines[ln][k] && j < MAX_COL; k++) lines[ln + 1][j++] = lines[ln][k];
    lines[ln + 1][j] = 0;
    lines[ln][col] = 0;
    nlines++;
}
static void del_line(int ln) {
    if (nlines <= 1) { lines[0][0] = 0; return; }
    for (int i = ln; i < nlines - 1; i++) scpy(lines[i], lines[i + 1]);
    nlines--;
}
static void merge_with_prev(int ln) {
    if (ln <= 0) return;
    int prev = line_len(ln - 1);
    int j = 0;
    for (int i = 0; lines[ln][i] && prev + j < MAX_COL; i++) lines[ln - 1][prev + j++] = lines[ln][i];
    lines[ln - 1][prev + j] = 0;
    del_line(ln);
}

// ---------------- 光标移动 ----------------
static void mv_left(void)  { if (cur_col > 0) cur_col--; }
static void mv_right(void) { if (cur_col < line_len(cur_line)) cur_col++; }
static void mv_up(void) {
    if (cur_line > 0) { cur_line--; if (cur_col > line_len(cur_line)) cur_col = line_len(cur_line); }
}
static void mv_down(void) {
    if (cur_line < nlines - 1) { cur_line++; if (cur_col > line_len(cur_line)) cur_col = line_len(cur_line); }
}
static int is_word_char(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9') || c == '_';
}
static void mv_word_forward(void) {
    int len = line_len(cur_line);
    if (cur_col < len) {
        if (is_word_char(lines[cur_line][cur_col])) {
            while (cur_col < len && is_word_char(lines[cur_line][cur_col])) cur_col++;
        } else {
            while (cur_col < len && !is_word_char(lines[cur_line][cur_col])) cur_col++;
        }
    }
    if (cur_col >= len && cur_line < nlines - 1) { cur_line++; cur_col = 0; }
}
static void mv_word_backward(void) {
    if (cur_col > 0) {
        int i = cur_col - 1;
        while (i >= 0 && !is_word_char(lines[cur_line][i])) i--;
        while (i >= 0 && is_word_char(lines[cur_line][i])) i--;
        cur_col = i + 1;
    } else if (cur_line > 0) {
        cur_line--;
        cur_col = line_len(cur_line);
        mv_word_backward();
    }
}
static void mv_word_end(void) {
    int len = line_len(cur_line);
    if (cur_col < len) {
        if (!is_word_char(lines[cur_line][cur_col])) {
            while (cur_col < len && !is_word_char(lines[cur_line][cur_col])) cur_col++;
        }
        while (cur_col + 1 < len && is_word_char(lines[cur_line][cur_col + 1])) cur_col++;
    }
    if (cur_col >= len && cur_line < nlines - 1) { cur_line++; cur_col = 0; }
}
static void page_up(void) {
    cur_line -= (page_h - 3);
    if (cur_line < 0) cur_line = 0;
    if (cur_col > line_len(cur_line)) cur_col = line_len(cur_line);
}
static void page_down(void) {
    cur_line += (page_h - 3);
    if (cur_line >= nlines) cur_line = nlines - 1;
    if (cur_col > line_len(cur_line)) cur_col = line_len(cur_line);
}

// ---------------- 渲染 ----------------
// 渲染目标是一个 W*H 字符矩阵, 由本编辑器与上一帧做行 diff,
// 只把变化的行以 ANSI 转义序列 (定位 + 文本 + 清行尾) 发给宿主侧
// terminal 模拟器; 终端模拟器负责按行增量刷新到宿主终端。
static char prev_page[512 * 200];
static int full_draw = 1;    // 首次渲染/尺寸变化时整屏输出
static int row_diff(const char* a, const char* b, int n) {
    for (int i = 0; i < n; i++) if (a[i] != b[i]) return 1;
    return 0;
}
static void row_append_str(char* row, int* p, const char* s) {
    for (int i = 0; s[i] && *p < page_w; i++) row[(*p)++] = s[i];
}
static void row_append_num(char* row, int* p, int v) {
    char tmp[12];
    int n = 0;
    if (v == 0) tmp[n++] = '0';
    while (v) { tmp[n++] = '0' + (v % 10); v /= 10; }
    for (int i = n - 1; i >= 0 && *p < page_w; i--) row[(*p)++] = tmp[i];
}
static void fill_row(char* row, const char* text, int start) {
    int written = 0;
    if (text) {
        for (int i = start; i < start + page_w; i++) {
            char c = text[i];
            if (!c) break;
            row[written++] = c;
        }
    }
    while (written < page_w) { row[written++] = ' '; }
}
static void set_msg(const char* s) {
    int i = 0;
    while (s[i] && i < page_w - 1 && i < 511) { msg[i] = s[i]; i++; }
    msg[i] = 0;
}
static void render(void) {
    // 终端尺寸变化检测: 变化后整屏重绘
    {
        unsigned int tw = 0, th = 0;
        tty_size_sys(&tw, &th);
        if (tw >= 10 && tw <= 512 && th >= 5 && th <= 200 &&
            ((int)tw != page_w || (int)th != page_h)) {
            page_w = (int)tw; page_h = (int)th;
            full_draw = 1;
        }
    }
    if (cur_line < top) top = cur_line;
    if (cur_line > top + (page_h - 3)) top = cur_line - (page_h - 3);
    if (top < 0) top = 0;
    if (cur_col < col_off) col_off = cur_col;
    if (cur_col > col_off + page_w - 1) col_off = cur_col - page_w + 1;
    if (col_off < 0) col_off = 0;

    char page[page_w * page_h];

    char* status = page;
    int sp = 0;
    const char* m = mode == MODE_INSERT ? "INSERT" :
                    mode == MODE_CMD    ? "COMMAND" :
                    mode == MODE_SEARCH ? "SEARCH" : "NORMAL";
    row_append_str(status, &sp, "-- ");
    row_append_str(status, &sp, m);
    row_append_str(status, &sp, " -- ");
    for (int i = 0; filename[i] && sp < (page_w - 30); i++) status[sp++] = filename[i];
    status[sp++] = ' ';
    status[sp++] = dirty ? '*' : ' ';
    row_append_str(status, &sp, "L"); row_append_num(status, &sp, cur_line + 1);
    status[sp++] = '/'; row_append_num(status, &sp, nlines);
    row_append_str(status, &sp, " C"); row_append_num(status, &sp, cur_col + 1);
    while (sp < page_w) status[sp++] = ' ';

    for (int r = 0; r < page_h - 2; r++) {
        int ln = top + r;
        char* row = page + (1 + r) * page_w;
        if (ln < nlines) fill_row(row, lines[ln], col_off);
        else             fill_row(row, 0, 0);
    }

    char* mrow = page + (page_h - 1) * page_w;
    int mp = 0;
    // 左下角常驻显示当前模式
    const char* mlbl = mode == MODE_INSERT ? "-- INSERT -- " :
                       mode == MODE_CMD    ? "-- COMMAND -- " :
                       mode == MODE_SEARCH ? "-- SEARCH -- " : "-- NORMAL -- ";
    int mlbl_len = 0;
    while (mlbl[mlbl_len]) mlbl_len++;
    for (int i = 0; mlbl[i] && mp < page_w; i++) mrow[mp++] = mlbl[i];
    if (mode == MODE_CMD) {
        mrow[mp++] = ':';
        for (int i = 0; i < cmd_len && mp < page_w; i++) mrow[mp++] = cmd_buf[i];
    } else if (mode == MODE_SEARCH) {
        mrow[mp++] = '/';
        for (int i = 0; i < pat_len && mp < page_w; i++) mrow[mp++] = pat_buf[i];
    } else {
        for (int i = 0; msg[i] && mp < page_w; i++) mrow[mp++] = msg[i];
    }
    while (mp < page_w) mrow[mp++] = ' ';

    int crow, ccol;
    if (mode == MODE_CMD)      { crow = page_h; ccol = mlbl_len + 1 + cmd_len; }
    else if (mode == MODE_SEARCH) { crow = page_h; ccol = mlbl_len + 1 + pat_len; }
    else                       { crow = 2 + (cur_line - top); ccol = 1 + (cur_col - col_off); }
    if (ccol > page_w) ccol = page_w;
    if (crow > page_h) crow = page_h;

    // 输出: 只发送内容变化的行
    for (int r = 0; r < page_h; r++) {
        char* row = page + r * page_w;
        if (!full_draw && !row_diff(row, prev_page + r * page_w, page_w)) continue;
        tput_pos(r + 1, 1);
        int len = page_w;
        while (len > 0 && row[len - 1] == ' ') len--;
        if (len > 0) writef(1, row, len);
        tput("\033[K");       // 清到行尾, 覆盖上一帧残留
        for (int i = 0; i < page_w; i++) prev_page[r * page_w + i] = row[i];
    }
    full_draw = 0;
    // 光标定位
    tput_pos(crow, ccol);
}

// ---------------- 文件读写 / 保存 ----------------
static int save(void) {
    char text[MAX_TEXT + 1];
    int p = 0;
    for (int i = 0; i < nlines; i++) {
        for (int j = 0; lines[i][j]; j++) text[p++] = lines[i][j];
        text[p++] = '\n';
    }
    text[p] = 0;
    int fd = openf(filename);
    if (fd < 0) {
        unsigned ino = 0;
        fd = createf(filename, 'f', &ino);
        if (fd < 0) return 0;
    }
    int n = writef(fd, text, p);
    closef(fd);
    if (n < p) return 0;
    dirty = 0;
    return 1;
}
static void load(void) {
    nlines = 1; lines[0][0] = 0;
    cur_line = 0; cur_col = 0; top = 0; col_off = 0;
    dirty = 0;

    int fd = openf(filename);
    if (fd < 0) { set_msg("new file"); return; }

    struct { char name[128]; unsigned size; unsigned start; char type; char _pad[3]; } ino;
    finfo(fd, &ino);

    char buf[MAX_TEXT + 1];
    int total = 0, n;
    while (total < MAX_TEXT && (n = readf(fd, buf + total, MAX_TEXT - total)) > 0) total += n;
    closef(fd);
    buf[total] = 0;

    nlines = 0;
    int s = 0;
    while (s < total && nlines < MAX_LINES) {
        int e = s;
        while (e < total && buf[e] != '\n') e++;
        int len = e - s; if (len > MAX_COL) len = MAX_COL;
        for (int i = 0; i < len; i++) lines[nlines][i] = buf[s + i];
        lines[nlines][len] = 0;
        nlines++;
        s = e + 1;
    }
    if (nlines == 0) { nlines = 1; lines[0][0] = 0; }

    char tmp[160];
    int t = 0;
    for (int i = 0; filename[i] && t < 100; i++) tmp[t++] = filename[i];
    tmp[t++] = ' ';
    { char num[12]; int nn = 0;
      int v = nlines; if (v == 0) num[nn++] = '0';
      while (v) { num[nn++] = '0' + (v % 10); v /= 10; }
      for (int i = nn - 1; i >= 0; i--) tmp[t++] = num[i]; }
    tmp[t++] = ' ';
    for (int i = 0; "lines"[i]; i++) tmp[t++] = "lines"[i];
    tmp[t] = 0;
    set_msg(tmp);
}

// ---------------- 搜索 ----------------
static int match_at(int ln, int col, const char* pat, int plen) {
    for (int j = 0; j < plen; j++) {
        if (lines[ln][col + j] != pat[j]) return 0;
    }
    return 1;
}
static void do_search(int forward) {
    const char* pat = last_pat;
    int plen = slen(pat);
    if (plen == 0) { set_msg("no search pattern"); return; }

    int len = line_len(cur_line);
    if (forward) {
        for (int c = cur_col + 1; c + plen <= len; c++) {
            if (match_at(cur_line, c, pat, plen)) { cur_col = c; return; }
        }
    } else {
        for (int c = cur_col - 1; c >= 0; c--) {
            if (c + plen <= len && match_at(cur_line, c, pat, plen)) { cur_col = c; return; }
        }
    }
    int i = cur_line;
    for (int step = 0; step < nlines - 1; step++) {
        if (forward) { i++; if (i >= nlines) i = 0; }
        else         { i--; if (i < 0) i = nlines - 1; }
        len = line_len(i);
        if (forward) {
            for (int c = 0; c + plen <= len; c++) {
                if (match_at(i, c, pat, plen)) { cur_line = i; cur_col = c; goto found; }
            }
        } else {
            for (int c = len - plen; c >= 0; c--) {
                if (match_at(i, c, pat, plen)) { cur_line = i; cur_col = c; goto found; }
            }
        }
    }
    set_msg("pattern not found");
    return;
found:
    {
        char tmp[32];
        int t = 0;
        for (int i = 0; "match at line "[i]; i++) tmp[t++] = "match at line "[i];
        { char num[12]; int nn = 0;
          int v = cur_line + 1; if (v == 0) num[nn++] = '0';
          while (v) { num[nn++] = '0' + (v % 10); v /= 10; }
          for (int i = nn - 1; i >= 0; i--) tmp[t++] = num[i]; }
        tmp[t] = 0;
        set_msg(tmp);
    }
}

// ---------------- 命令模式 ----------------
static void exec_cmd(void) {
    cmd_buf[cmd_len] = 0;
    if (scmp(cmd_buf, "w")) {
        if (save()) set_msg("written");
        else set_msg("write failed");
    } else if (scmp(cmd_buf, "wq") || scmp(cmd_buf, "wq!")) {
        if (save()) quit = 1;
        else set_msg("write failed");
    } else if (scmp(cmd_buf, "q")) {
        if (dirty) set_msg("no write since last change (:q! to discard)");
        else quit = 1;
    } else if (scmp(cmd_buf, "q!")) {
        quit = 1;
    } else if (cmd_buf[0] == 'e' && (cmd_buf[1] == ' ' || cmd_buf[1] == '!' || cmd_buf[1] == 0)) {
        int bang = (cmd_buf[1] == '!');
        int sp = bang ? 2 : 1;
        if (dirty && !bang) { set_msg("no write since last change (:e! to discard)"); return; }
        if (cmd_buf[sp] == 0) { set_msg("usage: :e <path>"); return; }
        scpy_n(filename, cmd_buf + sp, sizeof(filename));
        load();
    } else if (cmd_buf[0] == 'w' && cmd_buf[1] == ' ') {
        scpy_n(filename, cmd_buf + 2, sizeof(filename));
        if (save()) set_msg("written");
        else set_msg("write failed");
    } else {
        int all_digit = cmd_buf[0] != 0;
        for (int i = 0; cmd_buf[i]; i++) {
            if (cmd_buf[i] < '0' || cmd_buf[i] > '9') { all_digit = 0; break; }
        }
        if (all_digit) {
            int line_no = 0;
            for (int i = 0; cmd_buf[i]; i++) line_no = line_no * 10 + (cmd_buf[i] - '0');
            if (line_no >= 1) {
                cur_line = line_no - 1;
                if (cur_line >= nlines) cur_line = nlines - 1;
                cur_col = 0;
            }
        } else {
            set_msg("unknown command");
        }
    }
}

// ---------------- 按键处理 ----------------
// 读取一个按键: 普通字符原样返回, ANSI 转义序列映射为负值特殊键
static int read_key(void) {
    static char pend[8];
    static int pend_n = 0, pend_r = 0;
    if (pend_r < pend_n) return (unsigned char)pend[pend_r++];

    char c;
    if (readf(0, &c, 1) <= 0) return 0;
    if (c != 27) return (unsigned char)c;

    char c2;
    if (readf(0, &c2, 1) <= 0) return 27;          // 单独的 ESC
    if (c2 != '[') {                               // ESC 后紧跟普通键 (快速按键)
        pend[0] = c2; pend_n = 1; pend_r = 0;
        return 27;
    }
    char c3;
    if (readf(0, &c3, 1) <= 0) { pend[0] = '['; pend_n = 1; pend_r = 0; return 27; }
    switch (c3) {
        case 'A': return KEY_UP;
        case 'B': return KEY_DOWN;
        case 'C': return KEY_RIGHT;
        case 'D': return KEY_LEFT;
        case 'H': return KEY_HOME;
        case 'F': return KEY_END;
        case '5': case '6': case '3': case '1': case '4': case '7': case '8': {
            char c4;
            int tries = 400;
            while (tries-- > 0) {
                if (readf(0, &c4, 1) > 0) break;
            }
            if (tries >= 0) {
                switch (c3) {
                    case '5': return KEY_PGUP;
                    case '6': return KEY_PGDN;
                    case '3': return KEY_DEL;
                    case '1': case '7': return KEY_HOME;
                    case '4': case '8': return KEY_END;
                }
            }
            pend[0] = '['; pend[1] = c3; pend_n = 2; pend_r = 0;
            return 27;
        }
        default:
            pend[0] = '['; pend[1] = c3; pend_n = 2; pend_r = 0;
            return 27;
    }
}

static void normal_key(int k) {
    static int pend = 0;     // 等待 g 的第二个键

    if (k == KEY_DEL) k = 'x';

    if (pend == 'g') {
        if (k == 'g') { cur_line = 0; cur_col = 0; top = 0; col_off = 0; pend = 0; return; }
        pend = 0;
    }

    switch (k) {
        case 'h': case KEY_LEFT:  mv_left(); break;
        case 'l': case KEY_RIGHT: mv_right(); break;
        case 'k': case KEY_UP:    mv_up(); break;
        case 'j': case KEY_DOWN:  mv_down(); break;
        case 'w': case 'W': mv_word_forward(); break;
        case 'b': case 'B': mv_word_backward(); break;
        case 'e': case 'E': mv_word_end(); break;
        case '0': cur_col = 0; break;
        case '^': {
            int len = line_len(cur_line), c = 0;
            while (c < len && lines[cur_line][c] == ' ') c++;
            cur_col = c;
            break;
        }
        case '$': cur_col = line_len(cur_line); break;
        case KEY_HOME: cur_col = 0; break;
        case KEY_END: cur_col = line_len(cur_line); break;
        case KEY_PGUP: page_up(); break;
        case KEY_PGDN: page_down(); break;
        case 'g': pend = 'g'; return;
        case 'G': cur_line = nlines - 1; cur_col = 0; break;
        case 'i': mode = MODE_INSERT; break;
        case 'a': mode = MODE_INSERT; if (cur_col < line_len(cur_line)) cur_col++; break;
        case 'I': mode = MODE_INSERT; cur_col = 0; break;
        case 'A': mode = MODE_INSERT; cur_col = line_len(cur_line); break;
        case 'o':
            newline_after(cur_line);
            cur_line++; cur_col = 0; dirty = 1; mode = MODE_INSERT;
            break;
        case 'O':
            newline_before(cur_line);
            cur_col = 0; dirty = 1; mode = MODE_INSERT;
            break;
        case 'x':
            if (cur_col < line_len(cur_line)) {
                del_char(cur_line, cur_col);
                dirty = 1;
            }
            break;
        case 'X':
            if (cur_col > 0) {
                del_char(cur_line, cur_col - 1);
                cur_col--;
                dirty = 1;
            }
            break;
        case '/':
            mode = MODE_SEARCH;
            pat_len = 0;
            pat_buf[0] = 0;
            break;
        case ':':
            mode = MODE_CMD;
            cmd_len = 0;
            cmd_buf[0] = 0;
            break;
        default: break;
    }
}

static void insert_key(int k) {
    switch (k) {
        case 27: mode = MODE_NORMAL; break;
        case '\n': case '\r':
            split_line(cur_line, cur_col);
            cur_line++; cur_col = 0;
            dirty = 1;
            break;
        case 127: case 8:
            if (cur_col > 0) {
                del_char(cur_line, cur_col - 1);
                cur_col--;
                dirty = 1;
            } else if (cur_line > 0) {
                int pl = line_len(cur_line - 1);
                merge_with_prev(cur_line);
                cur_line--; cur_col = pl;
                dirty = 1;
            }
            break;
        case 9: {
            for (int i = 0; i < 4; i++) {
                if (line_len(cur_line) >= MAX_COL) break;
                ins_char(cur_line, cur_col, ' ');
                cur_col++;
            }
            dirty = 1;
            break;
        }
        case KEY_LEFT:  mv_left(); break;
        case KEY_RIGHT: mv_right(); break;
        case KEY_UP:    mv_up(); break;
        case KEY_DOWN:  mv_down(); break;
        case KEY_HOME: cur_col = 0; break;
        case KEY_END: cur_col = line_len(cur_line); break;
        case KEY_DEL:
            if (cur_col < line_len(cur_line)) {
                del_char(cur_line, cur_col);
                dirty = 1;
            }
            break;
        case KEY_PGUP: page_up(); break;
        case KEY_PGDN: page_down(); break;
        default:
            if (k >= 32 && k < 127) {
                ins_char(cur_line, cur_col, (char)k);
                cur_col++;
                dirty = 1;
            }
            break;
    }
}

static void cmd_key(int k) {
    if (k == 27) { mode = MODE_NORMAL; return; }
    if (k == '\n' || k == '\r') {
        exec_cmd();
        mode = MODE_NORMAL;
        return;
    }
    if (k == 127 || k == 8) {
        if (cmd_len > 0) { cmd_len--; cmd_buf[cmd_len] = 0; }
        return;
    }
    if (k >= 32 && k < 127 && cmd_len < PROMPT_LEN) {
        cmd_buf[cmd_len++] = (char)k;
        cmd_buf[cmd_len] = 0;
    }
}

static void search_key(int k) {
    if (k == 27) { mode = MODE_NORMAL; return; }
    if (k == '\n' || k == '\r') {
        if (pat_buf[0]) scpy(last_pat, pat_buf);
        do_search(1);
        mode = MODE_NORMAL;
        return;
    }
    if (k == 127 || k == 8) {
        if (pat_len > 0) { pat_len--; pat_buf[pat_len] = 0; }
        return;
    }
    if (k >= 32 && k < 127 && pat_len < PROMPT_LEN) {
        pat_buf[pat_len++] = (char)k;
        pat_buf[pat_len] = 0;
    }
}

// ---------------- 入口 ----------------
int main(int argc, char* argv[]) {
    if (argc >= 2 && argv[1] && argv[1][0]) scpy_n(filename, argv[1], sizeof(filename));
    {
        unsigned int tw = 0, th = 0;
        tty_size_sys(&tw, &th);
        if (tw >= 10 && tw <= 512 && th >= 5 && th <= 200) { page_w = (int)tw; page_h = (int)th; }
    }
    load();
    // 进入备用缓冲: shell 的主屏由终端模拟器保留, 退出时切回
    tput("\033[?1049h");
    render();
    // 进入输入循环前丢弃积压输入, 避免加载期间提前输入被当成按键吃掉
    flush_input_sys();
    while (!quit) {
        int k = read_key();
        if (k == 0) {
            // 无输入: 短暂忙等, 避免疯狂发起系统调用拖慢模拟器
            for (volatile int i = 0; i < 20000; i++);
            continue;
        }
        if (mode == MODE_NORMAL) normal_key(k);
        else if (mode == MODE_INSERT) insert_key(k);
        else if (mode == MODE_CMD) cmd_key(k);
        else if (mode == MODE_SEARCH) search_key(k);
        render();
    }
    // 退出前切回主缓冲, 由终端模拟器恢复 shell 界面
    tput("\033[?1049l");
    exit_proc();
    return 0;
}
