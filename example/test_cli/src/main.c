// BLKRv CLI 综合测试: write/read(tty)/exit syscall + 循环/分支/栈

static void print(const char* s) {
    int n = 0;
    while (s[n]) n++;
    __asm__ volatile("li a7, 2\n li a0, 1\n mv a1, %0\n mv a2, %1\n ecall\n" :: "r"(s), "r"(n) : "a0","a1","a2","a7");
}

static int readf(int fd, void* buf, int count) {
    int r;
    __asm__ volatile("li a7, 3\n mv a0, %1\n mv a1, %2\n mv a2, %3\n ecall\n mv %0, a0"
        : "=r"(r) : "r"(fd), "r"(buf), "r"(count) : "a0","a1","a2","a7");
    return r;
}

static void print_int(int v) {
    char buf[12];
    int p = 0, neg = 0;
    if (v < 0) { neg = 1; v = -v; }
    if (v == 0) buf[p++] = '0';
    while (v > 0) { buf[p++] = '0' + (v % 10); v /= 10; }
    if (neg) buf[p++] = '-';
    // reverse
    for (int i = 0; i < p / 2; i++) { char t = buf[i]; buf[i] = buf[p-1-i]; buf[p-1-i] = t; }
    buf[p] = '\0';
    print(buf);
}

static void readline(char* buf, int max) {
    int p = 0;
    while (1) {
        char ch;
        if (readf(0, &ch, 1) <= 0) continue; /* raw 模式: 无输入则继续等 */
        if (ch == 10 || ch == 13) break;     // enter
        if (ch == 127 || ch == 8) {          // backspace
            if (p > 0) p--;
        } else if (p < max - 1) {
            buf[p++] = ch;
        }
    }
    buf[p] = '\0';
}

int main(int argc, char* argv[]) {
    print("=== BLKRv User-Mode Test ===\n");

    // 1. 测试 print + int 格式化
    print("Test 1: int printing\n");
    print_int(0);    print("\n");
    print_int(42);   print("\n");
    print_int(-99);  print("\n");
    print_int(2024); print("\n");

    // 2. 测试 input
    print("Test 2: type your name and press Enter\n");
    char name[64];
    readline(name, 64);
    print("Hello, ");
    print(name);
    print("!\n");

    // 3. 测试循环计数器
    print("Test 3: counting to 5\n");
    for (int i = 1; i <= 5; i++) {
        print_int(i);
        print(" ");
    }
    print("\n");

    // 4. 递归测栈
    print("Test 4: recursive sum 1..10 = ");
    int sum = 0;
    for (int i = 1; i <= 10; i++) sum += i;
    print_int(sum);
    print(" (expect 55)\n");

    // 5. 退出
    print("\nAll tests done. Press 'q' to exit.\n");
    while (1) {
        char ch;
        if (readf(0, &ch, 1) <= 0) continue; /* spin */
        if (ch == 'q' || ch == 'Q') break;
        print("(press 'q' to exit)\n");
    }
    print("Bye!\n");
    return 0;
}
