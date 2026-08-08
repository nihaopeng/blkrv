// BLKRv ls - 列出目录内容
//   ls         仅显示名字 (多列, 含 "." 与 "..")
//   ls -l      显示 类型/名字/大小 (每行一个)
//   ls [路径]  指定目录, 默认为 /
// 标准库: blkrv.h (open/finfo/finoid/read/write/tty_size/str_len/str_cpy)

#include "blkrv.h"

#define MAX_ENT 256

typedef struct { char name[128]; unsigned size; char type; } ent_t;
static ent_t ents[MAX_ENT];
static int ent_n = 0;

typedef struct { char name[128]; unsigned size; unsigned start; char type; char _pad[3]; } inode_t;

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

// 名字 + 目录 / 后缀长度 (不含结尾 \0)
static unsigned name_w(ent_t* e) {
    unsigned l = (unsigned)str_len(e->name);
    if (e->type == 'd') l += 1;
    return l;
}

// 简单模式: 名字单元格 "name/  " (目录带 /)
static void put_name_cell(char* out, int* op, ent_t* e, unsigned max_name, int last) {
    unsigned name_len = 0;
    for (int k = 0; e->name[k]; k++) { out[(*op)++] = e->name[k]; name_len++; }
    if (e->type == 'd') { out[(*op)++] = '/'; name_len++; }
    while (name_len < max_name) { out[(*op)++] = ' '; name_len++; }
    if (!last) { out[(*op)++] = ' '; out[(*op)++] = ' '; }
}

// 详细模式: 一行 "d name/  (size)  "
static void put_long_line(char* out, int* op, ent_t* e, unsigned max_name, unsigned max_sz) {
    out[(*op)++] = (e->type == 'd') ? 'd' : 'f';
    out[(*op)++] = ' ';
    unsigned name_len = 0;
    for (int k = 0; e->name[k]; k++) { out[(*op)++] = e->name[k]; name_len++; }
    if (e->type == 'd') { out[(*op)++] = '/'; name_len++; }
    while (name_len < max_name) { out[(*op)++] = ' '; name_len++; }
    out[(*op)++] = ' ';
    char num[12];
    num_to_str(e->size, num);
    unsigned nd = (unsigned)str_len(num);
    while (nd < max_sz) { out[(*op)++] = ' '; nd++; }
    for (int k = 0; num[k]; k++) out[(*op)++] = num[k];
    out[(*op)++] = '\n';
}

int main(int argc, char* argv[]) {
    ent_n = 0;   // 本加载器已清零 .bss, 这里仍显式初始化
    const char* path = "/";
    int long_mode = 0;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] != 0) {
            for (int j = 1; argv[i][j]; j++) {
                if (argv[i][j] == 'l') long_mode = 1;
                else { print("ls: unknown option -%c\n", argv[i][j]); return 1; }
            }
        } else {
            path = argv[i];   // 只取第一个路径参数
            break;
        }
    }

    int fd = open(path);
    if (fd < 0) { print("ls: open failed\n"); return 1; }

    inode_t ino;
    if (finfo(fd, &ino) < 0) { print("ls: finfo failed\n"); return 1; }

    // 非目录: 一行输出 名字 (详细模式附加大小)
    if (ino.type != 'd') {
        char line[256];
        int p = 0;
        for (int i = 0; ino.name[i]; i++) line[p++] = ino.name[i];
        if (long_mode) {
            line[p++] = ' ';
            char num[12];
            num_to_str(ino.size, num);
            for (int i = 0; num[i]; i++) line[p++] = num[i];
        }
        line[p++] = '\n';
        write(1, line, p);
        return 0;
    }

    // 第一步: 收集全部条目 (fd 偏移自动推进, 循环读直到读完)
    char buf[1024];
    int n;
    int truncated = 0;
    while (ent_n < MAX_ENT && (n = read(fd, buf, 1024)) > 0) {
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

    // 追加 "." (目录自身) 与 ".." (父目录), 便于 cd 导航
    if (ent_n + 2 <= MAX_ENT) {
        str_cpy(".", ents[ent_n].name);
        ents[ent_n].size = ino.size;
        ents[ent_n].type = 'd';
        ent_n++;
        str_cpy("..", ents[ent_n].name);
        ents[ent_n].size = 0;
        ents[ent_n].type = 'd';
        // 父目录大小: 由路径去掉最后一个分量推导, 失败则保持 0
        int pe = 0;
        while (path[pe]) pe++;
        while (pe > 1 && path[pe-1] == '/') pe--;
        int last = pe;
        while (last > 0 && path[last-1] != '/') last--;
        char parent[256];
        int pp = 0;
        if (last == 0) {
            parent[0] = '/'; parent[1] = 0;
        } else {
            for (int k = 0; k < last; k++) parent[pp++] = path[k];
            parent[pp] = 0;
        }
        int pfd = open(parent);
        if (pfd >= 0) {
            inode_t pin;
            if (finfo(pfd, &pin) >= 0) ents[ent_n].size = pin.size;
            close(pfd);
        }
        ent_n++;
    }

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
    unsigned max_name = 0;
    for (int i = 0; i < ent_n; i++) {
        unsigned l = name_w(&ents[i]);
        if (l > max_name) max_name = l;
    }

    // 第四步: 统一构建输出, 一次写完 (条目过多时按块刷出, 正常目录一次写完)
    static char out[16384];
    int op = 0;
    if (long_mode) {
        unsigned max_sz = 0;
        for (int i = 0; i < ent_n; i++) {
            char num[12];
            num_to_str(ents[i].size, num);
            unsigned d = (unsigned)str_len(num);
            if (d > max_sz) max_sz = d;
        }
        for (int i = 0; i < ent_n; i++) {
            put_long_line(out, &op, &ents[i], max_name, max_sz);
            if (op > 12000) {
                write(1, out, op);
                op = 0;
            }
        }
    } else {
        unsigned cell_w = max_name + 3;          // 名字 + 间隔
        unsigned cols = tw / cell_w;
        if (cols < 1) cols = 1;
        if (cols > (unsigned)ent_n) cols = (unsigned)ent_n;
        unsigned rows = ((unsigned)ent_n + cols - 1) / cols;
        for (unsigned r = 0; r < rows; r++) {
            for (unsigned c = 0; c < cols; c++) {
                unsigned idx = r * cols + c;
                if (idx >= (unsigned)ent_n) break;
                put_name_cell(out, &op, &ents[idx], max_name, (int)(c + 1 == cols));
            }
            out[op++] = '\n';
            if (op > 12000) {
                write(1, out, op);
                op = 0;
            }
        }
    }
    if (op > 0) write(1, out, op);
    if (truncated) print("ls: too many entries, truncated\n");
    return 0;
}
