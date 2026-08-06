// BLKRV rm — 删除文件/目录 (按 inode id)
static void print(const char* s) {
    int n = 0; while (s[n]) n++;
    __asm__ volatile("li a7, 5\n mv a0, %0\n mv a1, %1\n ecall\n" :: "r"(s), "r"(n) : "a0","a1","a7");
}
static int open(const char* path) {
    int r;
    __asm__ volatile("li a7, 0\n mv a0, %1\n ecall\n mv %0, a0" : "=r"(r) : "r"(path) : "a0","a7");
    return r;
}
static int del(unsigned id) {
    int r;
    __asm__ volatile("li a7, 1\n mv a0, %1\n ecall\n mv %0, a0" : "=r"(r) : "r"(id) : "a0","a7");
    return r;
}

int main(int argc, char* argv[]) {
    if (argc < 2) { print("Usage: rm <file>\n"); return 1; }
    int fd = open(argv[1]);
    if (fd < 0) { print("rm: not found\n"); return 1; }
    if (del(fd) < 0) { print("rm: failed (dir not empty?)\n"); return 1; }
    return 0;
}
