// BLKRV cat — 输出文件内容
static void print(const char* s) {
    int n = 0; while (s[n]) n++;
    __asm__ volatile("li a7, 2\n li a0, 1\n mv a1, %0\n mv a2, %1\n ecall\n" :: "r"(s), "r"(n) : "a0","a1","a2","a7");
}
static int open(const char* path) {
    int r;
    __asm__ volatile("li a7, 0\n mv a0, %1\n ecall\n mv %0, a0" : "=r"(r) : "r"(path) : "a0","a7");
    return r;
}
static int finfo(int fd, void* ino) {
    int r;
    __asm__ volatile("li a7, 20\n mv a0, %1\n mv a1, %2\n ecall\n mv %0, a0" : "=r"(r) : "r"(fd), "r"(ino) : "a0","a1","a7");
    return r;
}
static int readf(int fd, char* buf, int count) {
    int r;
    __asm__ volatile("li a7, 3\n mv a0, %1\n mv a1, %2\n mv a2, %3\n ecall\n mv %0, a0"
        : "=r"(r) : "r"(fd), "r"(buf), "r"(count) : "a0","a1","a2","a7");
    return r;
}

typedef struct { char name[128]; unsigned size; unsigned start; char type; char _pad[3]; } inode_t;

int main(int argc, char* argv[]) {
    if (argc < 2) { print("Usage: cat <file>\n"); return 1; }

    int fd = open(argv[1]);
    if (fd < 0) { print("cat: open failed\n"); return 1; }

    char buf[128];
    int n;
    while ((n = readf(fd, buf, 128)) > 0) {
        __asm__ volatile("li a7, 2\n li a0, 1\n mv a1, %0\n mv a2, %1\n ecall\n" :: "r"(buf), "r"(n) : "a0","a1","a2","a7");
    }
    return 0;
}
