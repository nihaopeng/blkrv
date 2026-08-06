// BLKRV clear — 清屏 (终端无转义支持, 用空行刷屏)
static void print(const char* s) {
    int n = 0; while (s[n]) n++;
    __asm__ volatile("li a7, 5\n mv a0, %0\n mv a1, %1\n ecall\n" :: "r"(s), "r"(n) : "a0","a1","a7");
}

int main(void) {
    for (int i = 0; i < 50; i++) print("\n");
    return 0;
}
