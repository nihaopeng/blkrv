// BLKRV ls — 列出目录内容
// syscall: write(1), open, finfo, read

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
static int finoid(unsigned id, void* ino) {
    int r;
    __asm__ volatile("li a7, 28\n mv a0, %1\n mv a1, %2\n ecall\n mv %0, a0" : "=r"(r) : "r"(id), "r"(ino) : "a0","a1","a7");
    return r;
}

typedef struct { char name[128]; unsigned size; unsigned start; char type; char _pad[3]; } inode_t;

static void print_hex(unsigned v) {
    char h[] = "0123456789ABCDEF";
    char buf[9]; buf[8] = 0;
    for (int i = 7; i >= 0; i--) { buf[i] = h[v & 0xf]; v >>= 4; }
    print(buf);
}

static void print_int(unsigned v) {
    char buf[12]; int p = 0;
    if (v == 0) buf[p++] = '0';
    while (v > 0) { buf[p++] = '0' + (v % 10); v /= 10; }
    for (int i = 0; i < p/2; i++) { char t = buf[i]; buf[i] = buf[p-1-i]; buf[p-1-i] = t; }
    buf[p] = 0; print(buf);
}

int main(int argc, char* argv[]) {
    // print("ls: list directory contents\n");
    const char* path = "/";
    if (argc > 1) path = argv[1];

    int fd = open(path);
    if (fd < 0) { print("ls: open failed\n"); return 1; }

    inode_t ino;
    if (finfo(fd, &ino) < 0) { print("ls: finfo failed\n"); return 1; }
    if (ino.type != 'd') {
        print(ino.name); print("\n");
        return 0;
    }

    char buf[1024];
    int rd = readf(fd, buf, 1024);
    if (rd < 0) { print("ls: read dir failed\n"); return 1; }
    // rd == 0: 空目录, 直接结束 (不是错误)

    // 每 4 字节是一个 inode_id
    for (int i = 0; i < rd; i += 4) {
        int id = *(int*)(buf + i);
        if (id == 0) continue;
        inode_t child;
        if (finoid(id, &child) < 0) continue;
        if (child.type == 0) continue;  // 已删除的幽灵条目
        print(child.type == 'd' ? "d " : "  ");
        print(child.name);
        print("  (");
        print_int(child.size);
        print(")\n");
    }
    return 0;
}
