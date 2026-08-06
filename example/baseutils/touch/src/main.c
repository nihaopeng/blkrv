// BLKRV touch — 创建空文件
static void print(const char* s) {
    int n = 0; while (s[n]) n++;
    __asm__ volatile("li a7, 2\n li a0, 1\n mv a1, %0\n mv a2, %1\n ecall\n" :: "r"(s), "r"(n) : "a0","a1","a2","a7");
}
static int create(const char* path, char type, unsigned* id) {
    int r;
    __asm__ volatile("li a7, 4\n mv a0, %1\n mv a1, %2\n mv a2, %3\n ecall\n mv %0, a0"
        : "=r"(r) : "r"(path), "r"(type), "r"(id) : "a0","a1","a2","a7");
    return r;
}

int main(int argc, char* argv[]) {
    if (argc < 2) { print("Usage: touch <file>\n"); return 1; }
    unsigned id;
    if (create(argv[1], 'f', &id) < 0) { print("touch: failed\n"); return 1; }
    return 0;
}
