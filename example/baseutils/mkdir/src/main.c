// BLKRV mkdir — 创建目录
static void print(const char* s) {
    int n = 0; while (s[n]) n++;
    __asm__ volatile("li a7, 5\n mv a0, %0\n mv a1, %1\n ecall\n" :: "r"(s), "r"(n) : "a0","a1","a7");
}
static int create(const char* path, char type, unsigned* id) {
    int r;
    __asm__ volatile("li a7, 4\n mv a0, %1\n mv a1, %2\n mv a2, %3\n ecall\n mv %0, a0"
        : "=r"(r) : "r"(path), "r"(type), "r"(id) : "a0","a1","a2","a7");
    return r;
}

int main(int argc, char* argv[]) {
    if (argc < 2) { print("Usage: mkdir <dir>\n"); return 1; }
    unsigned id;
    if (create(argv[1], 'd', &id) < 0) { print("mkdir: failed\n"); return 1; }
    return 0;
}
