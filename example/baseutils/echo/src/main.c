// BLKRV echo — 输出所有参数
static void print(const char* s) {
    int n = 0; while (s[n]) n++;
    __asm__ volatile("li a7, 2\n li a0, 1\n mv a1, %0\n mv a2, %1\n ecall\n" :: "r"(s), "r"(n) : "a0","a1","a2","a7");
}

int main(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        if (i > 1) print(" ");
        print(argv[i]);
    }
    print("\n");
    return 0;
}
