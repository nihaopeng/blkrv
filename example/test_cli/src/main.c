// BLKRv CLI 综合测试: write/read(tty)/exit syscall + 循环/分支/栈
#include "blkrv.h"

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

static void readline(char* buf, int max) {
    int p = 0;
    while (1) {
        char ch;
        if (read(0, &ch, 1) <= 0) continue; /* raw 模式: 无输入则继续等 */
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

    print("Test 1: int printing\n");
    print_int(0);    print("\n");
    print_int(42);   print("\n");
    print_int(-99);  print("\n");
    print_int(2024); print("\n");

    print("Test 2: type your name and press Enter\n");
    char name[64];
    readline(name, 64);
    print("Hello, ");
    print(name);
    print("!\n");

    print("Test 3: counting to 5\n");
    for (int i = 1; i <= 5; i++) {
        print_int(i);
        print(" ");
    }
    print("\n");

    print("Test 4: recursive sum 1..10 = ");
    int sum = 0;
    for (int i = 1; i <= 10; i++) sum += i;
    print_int(sum);
    print(" (expect 55)\n");

    print("\nAll tests done. Press 'q' to exit.\n");
    while (1) {
        char ch;
        if (read(0, &ch, 1) <= 0) continue; /* spin */
        if (ch == 'q' || ch == 'Q') break;
        print("(press 'q' to exit)\n");
    }
    print("Bye!\n");
    return 0;
}
