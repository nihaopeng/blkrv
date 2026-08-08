// BLKRV ls - 列出目录内容
// syscall: write(1), open, finfo, finoid, read, tty_size
// 流程: 先收集目录下全部条目 -> 按名字排序 -> 按终端宽度多列对齐 -> 一次输出。

#define MAX_ENT 256

typedef struct { char name[128]; unsigned size; char type; } ent_t;
static ent_t ents[MAX_ENT];
static int ent_n = 0;

static void write_out(const char* s, int n) {
    __asm__ volatile("li a7, 2\n li a0, 1\n mv a1, %0\n mv a2, %1\n ecall\n" :: "r"(s), "r"(n) : "a0","a1","a2","a7");
}
static void print(const char* s) {
    int n = 0; while (s[n]) n++;
    write_out(s, n);
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
static int tty_size(unsigned* w, unsigned* h) {
    int r;
    __asm__ volatile("li a7, 30\n mv a0, %1\n mv a1, %2\n ecall\n mv %0, a0"
        : "=r"(r) : "r"(w), "r"(h) : "a0","a1","a7");
    return r;
}

typedef struct { char name[128]; unsigned size; unsigned start; char type; char _pad[3]; } inode_t;

static int str_len(const char* s) { int n = 0; while (s[n]) n++; return n; }
static void str_cpy(const char* src, char* dst) {
    int i = 0; while (src[i]) { dst[i] = src[i]; i++; } dst[i] = 0;
}
// ASCII 字典序: <0 表示 a 排在 b 前
static int name_cmp(const char* a, const char* b) {
    int i = 0;
    while (a[i] && b[i] && a[i] == b[i]) i++;
    return (int)(unsigned char)a[i] - (int)(unsigned char)b[i];
}
static void num_to_str(unsigned v, char* out) {
    char tmp[12];
    int t = 0;
    if (v == 0) tmp[t++] = '0';
    while (v) { tmp[t++] = '0' + (v % 10); v /= 10; }
    int p = 0;
    while (t > 0) out[p++] = tmp[--t];
    out[p] = 0;
}

// 输出一个条目的单元格: "d name  (size)  " (大小右对齐)
static void put_cell(char* out, int* op, ent_t* e, unsigned max_name, unsigned max_sz, int last) {
    out[(*op)++] = (e->type == 'd') ? 'd' : ' ';
    out[(*op)++] = ' ';
    unsigned name_len = 0;
    for (int k = 0; e->name[k]; k++) { out[(*op)++] = e->name[k]; name_len++; }
    if (e->type == 'd') { out[(*op)++] = '/'; name_len++; }   // 目录带 / 后缀
    while (name_len < max_name) { out[(*op)++] = ' '; name_len++; }
    out[(*op)++] = ' '; out[(*op)++] = '(';
    char num[12];
    num_to_str(e->size, num);
    unsigned nd = (unsigned)str_len(num);
    while (nd < max_sz) { out[(*op)++] = ' '; nd++; }
    for (int k = 0; num[k]; k++) out[(*op)++] = num[k];
    out[(*op)++] = ')';
    if (!last) { out[(*op)++] = ' '; out[(*op)++] = ' '; }
}

int main(int argc, char* argv[]) {
    ent_n = 0;   // 显式清零: 本加载器不保证 .bss 归零
    const char* path = "/";
    if (argc > 1) path = argv[1];

    int fd = open(path);
    if (fd < 0) { print("ls: open failed\n"); return 1; }

    inode_t ino;
    if (finfo(fd, &ino) < 0) { print("ls: finfo failed\n"); return 1; }

    // 非目录: 单行输出 名字 (大小)
    if (ino.type != 'd') {
        char line[256];
        int p = 0;
        for (int i = 0; ino.name[i]; i++) line[p++] = ino.name[i];
        line[p++] = ' '; line[p++] = '(';
        char num[12];
        num_to_str(ino.size, num);
        for (int i = 0; num[i]; i++) line[p++] = num[i];
        line[p++] = ')'; line[p++] = '\n';
        write_out(line, p);
        return 0;
    }

    // 第一步: 收集全部条目 (fd 偏移自动推进, 循环读直到读完)
    char buf[1024];
    int n;
    int truncated = 0;
    while (ent_n < MAX_ENT && (n = readf(fd, buf, 1024)) > 0) {
        for (int i = 0; i + 4 <= n && ent_n < MAX_ENT; i += 4) {
            int id = (int)((buf[i] & 0xff) | ((buf[i+1] & 0xff) << 8)
                         | ((buf[i+2] & 0xff) << 16) | ((buf[i+3] & 0xff) << 24));
            if (id == 0) continue;
            inode_t child;
            if (finoid((unsigned)id, &child) < 0) continue;
            if (child.type == 0) continue;   // 已删除的幽灵条目
            str_cpy(child.name, ents[ent_n].name);
            ents[ent_n].size = child.size;
            ents[ent_n].type = child.type;
            ent_n++;
        }
        if (ent_n >= MAX_ENT) { truncated = 1; break; }
    }
    if (ent_n == 0) return 0;

    // 第二步: 按名字排序 (插入排序, 字段拷贝避免依赖 memcpy)
    for (int i = 1; i < ent_n; i++) {
        ent_t key;
        str_cpy(ents[i].name, key.name);
        key.size = ents[i].size;
        key.type = ents[i].type;
        int j = i - 1;
        while (j >= 0 && name_cmp(ents[j].name, key.name) > 0) {
            str_cpy(ents[j].name, ents[j + 1].name);
            ents[j + 1].size = ents[j].size;
            ents[j + 1].type = ents[j].type;
            j--;
        }
        str_cpy(key.name, ents[j + 1].name);
        ents[j + 1].size = key.size;
        ents[j + 1].type = key.type;
    }

    // 第三步: 计算列宽 (终端宽度来自 tty_size, 失败回退 80)
    unsigned tw = 80, th = 24;
    if (tty_size(&tw, &th) < 0 || tw < 10) tw = 80;
    if (tw > 512) tw = 512;
    unsigned max_name = 0, max_sz = 0;
    for (int i = 0; i < ent_n; i++) {
        unsigned l = (unsigned)str_len(ents[i].name);
        if (ents[i].type == 'd') l += 1;
        if (l > max_name) max_name = l;
        char num[12];
        num_to_str(ents[i].size, num);
        unsigned d = (unsigned)str_len(num);
        if (d > max_sz) max_sz = d;
    }
    unsigned cell_w = 2 + max_name + 1 + 2 + max_sz + 2;   // 标记+名字+(大小)+间隔
    unsigned cols = tw / cell_w;
    if (cols < 1) cols = 1;
    if (cols > (unsigned)ent_n) cols = (unsigned)ent_n;
    unsigned rows = ((unsigned)ent_n + cols - 1) / cols;

    // 第四步: 统一构建输出, 一次写完 (条目过多时按块刷出, 正常目录一次写完)
    static char out[16384];
    int op = 0;
    for (unsigned r = 0; r < rows; r++) {
        for (unsigned c = 0; c < cols; c++) {
            unsigned idx = r * cols + c;
            if (idx >= (unsigned)ent_n) break;
            put_cell(out, &op, &ents[idx], max_name, max_sz, (int)(c + 1 == cols));
        }
        out[op++] = '\n';
        if (op > 12000) {
            write_out(out, op);
            op = 0;
        }
    }
    if (op > 0) write_out(out, op);
    if (truncated) print("ls: too many entries, truncated\n");
    return 0;
}
