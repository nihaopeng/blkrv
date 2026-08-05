// BLKRv User Shell — 由调度器加载为 pid=1
// 编译: make -C example/shell

static void print(const char* s) {
    int n = 0;
    while (s[n]) n++;
    __asm__ volatile("li a7, 5\n mv a0, %0\n mv a1, %1\n ecall\n" :: "r"(s), "r"(n) : "a0","a1","a7");
}

static void print_int(int v) {
    char buf[12];
    int p = 0, neg = 0;
    if (v < 0) { neg = 1; v = -v; }
    if (v == 0) buf[p++] = '0';
    while (v > 0) { buf[p++] = '0' + (v % 10); v /= 10; }
    if (neg) buf[p++] = '-';
    for (int i = 0; i < p / 2; i++) { char t = buf[i]; buf[i] = buf[p-1-i]; buf[p-1-i] = t; }
    buf[p] = '\0';
    print(buf);
}

// 从键盘读一行 (忙等) — 用 kbhit/vgetch syscall
static int kbhit(void) {
    int r;
    __asm__ volatile("li a7, 8\n ecall\n mv %0, a0" : "=r"(r) :: "a0","a7");
    return r;
}
static char vgetch(void) {
    char r;
    __asm__ volatile("li a7, 6\n ecall\n mv %0, a0" : "=r"(r) :: "a0","a7");
    return r;
}

static void readline(char* buf, int max) {
    int p = 0;
    while (1) {
        while (!kbhit()) { /* spin */ }
        char ch = vgetch();
        if (ch == 0) continue;
        if (ch == 10 || ch == 13) break;
        if (ch == 127 || ch == 8) {
            if (p > 0) { p--; print("\b \b"); }
        } else if (p < max - 1) {
            buf[p++] = ch;
            char tmp[2] = {ch, 0};
            print(tmp);
        }
    }
    buf[p] = '\0';
    print("\n");
}

int main(int argc, char* argv[]) {
    print("\n=== BLKRv Shell ===\n");
    print("type 'exit' to quit, 'echo xxx' to echo\n");

    char line[128];
    while (1) {
        print("> ");
        readline(line, 128);

        if (line[0] == 0) continue;

        // exit command
        if (line[0] == 'e' && line[1] == 'x' && line[2] == 'i' && line[3] == 't' && line[4] == 0) {
            print("Goodbye!\n");
            break;
        }

        // echo command
        if (line[0] == 'e' && line[1] == 'c' && line[2] == 'h' && line[3] == 'o' && line[4] == ' ') {
            print(line + 5);
            print("\n");
            continue;
        }

        // help
        if (line[0] == 'h' && line[1] == 'e' && line[2] == 'l' && line[3] == 'p' && line[4] == 0) {
            print("Commands: help, echo <msg>, exit\n");
            continue;
        }

        print("unknown: ");
        print(line);
        print("\n");
    }
    return 0;
}
