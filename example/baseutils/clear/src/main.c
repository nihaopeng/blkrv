// BLKRV clear — 发送 ANSI 清屏序列, 由宿主侧 terminal 模拟器处理
static int write_str(const char* s) {
    int n = 0; while (s[n]) n++;
    __asm__ volatile("li a7, 2\n li a0, 1\n mv a1, %0\n mv a2, %1\n ecall\n"
        :: "r"(s), "r"(n) : "a0","a1","a2","a7");
    return n;
}

int main(void) {
    write_str("CLRMARK");       // 调试: 验证 clear 进程是否执行了自己的 main
    write_str("\033[2J\033[H");
    return 0;
}
