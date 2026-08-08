// BLKRv User Shell — pid=1
// 内建: cd / pwd / help / exit (cd 必须内建, 子进程无法改变父进程工作目录)
// 外部程序: echo / ls / cat / mkdir / touch / rm / clear (位于 /bin)

#define MAX_PATH 256

static void print(const char* s) {
    int n = 0; while (s[n]) n++;
    __asm__ volatile("li a7, 2\n li a0, 1\n mv a1, %0\n mv a2, %1\n ecall\n" :: "r"(s), "r"(n) : "a0","a1","a2","a7");
}
static int openf(const char* path) {
    int r;
    __asm__ volatile("li a7, 0\n mv a0, %1\n ecall\n mv %0, a0" : "=r"(r) : "r"(path) : "a0","a7");
    return r;
}
static int finfo(int fd, void* ino) {
    int r;
    __asm__ volatile("li a7, 20\n mv a0, %1\n mv a1, %2\n ecall\n mv %0, a0" : "=r"(r) : "r"(fd), "r"(ino) : "a0","a1","a7");
    return r;
}
static int spawn(unsigned inode_id, char** argv, unsigned argc) {
    int r;
    __asm__ volatile("li a7, 26\n mv a0, %1\n mv a1, %2\n mv a2, %3\n ecall\n mv %0, a0"
        : "=r"(r) : "r"(inode_id), "r"(argv), "r"(argc) : "a0","a1","a2","a7");
    return r;
}
static int waitpid(int pid) {
    int r;
    __asm__ volatile("li a7, 27\n mv a0, %1\n ecall\n mv %0, a0" : "=r"(r) : "r"(pid) : "a0","a7");
    return r;
}
static int closef(int fd) {
    int r;
    __asm__ volatile("li a7, 25\n mv a0, %1\n ecall\n mv %0, a0" : "=r"(r) : "r"(fd) : "a0","a7");
    return r;
}
static int readf(int fd, void* buf, int count) {
    int r;
    __asm__ volatile("li a7, 3\n mv a0, %1\n mv a1, %2\n mv a2, %3\n ecall\n mv %0, a0"
        : "=r"(r) : "r"(fd), "r"(buf), "r"(count) : "a0","a1","a2","a7");
    return r;
}

typedef struct { char name[128]; unsigned size; unsigned start; char type; char _pad[3]; } inode_t;

static int str_len(const char* s) { int n=0; while(s[n])n++; return n; }
static int str_cmp(const char* a, const char* b) {
    int la=str_len(a), lb=str_len(b);
    if(la!=lb) return 0;
    for(int i=0;i<la;i++) if(a[i]!=b[i]) return 0;
    return 1;
}
static void str_cpy(const char* src, char* dst) {
    int i=0; while(src[i]){ dst[i]=src[i]; i++; } dst[i]=0;
}
static void str_cat(char* dst, const char* src) {
    int i=str_len(dst), j=0; while(src[j]){ dst[i++]=src[j++]; } dst[i]=0;
}

static char cwd[MAX_PATH];

// 规范化绝对路径: 处理 . 与 .. 组件, 结果写入 out
static void norm_path(const char* in, char* out) {
    char stack[16][128];
    int top = 0;
    char comp[128];
    int p = 0;
    for (int i = 0; ; i++) {
        char c = in[i];
        if (c == '/' || c == 0) {
            if (p > 0) {
                comp[p] = 0;
                if (str_cmp(comp, ".")) { /* 当前目录 */ }
                else if (str_cmp(comp, "..")) { if (top > 0) top--; }
                else { if (top < 16) str_cpy(comp, stack[top++]); }
                p = 0;
            }
            if (c == 0) break;
        } else {
            if (p < 127) comp[p++] = c;   // 组件超长截断, 防止越界
        }
    }
    out[0] = '/';
    int o = 1;
    for (int i = 0; i < top && o < MAX_PATH - 2; i++) {
        for (int j = 0; stack[i][j] && o < MAX_PATH - 2; j++) out[o++] = stack[i][j];
        out[o++] = '/';
    }
    if (o > 1) o--;        // 去掉末尾 '/'
    out[o] = 0;
}

// 把 src 解析为绝对路径到 out (基于 cwd)
static void resolve_path(const char* src, char* out) {
    char tmp[MAX_PATH];
    if (src[0] == '/') {
        str_cpy(src, tmp);
    } else {
        if (str_cmp(cwd, "/")) {
            tmp[0]='/'; tmp[1]=0;
            str_cat(tmp, src);
        } else {
            str_cpy(cwd, tmp);
            int l = str_len(tmp);
            tmp[l]='/'; tmp[l+1]=0;
            str_cat(tmp, src);
        }
    }
    norm_path(tmp, out);
}

static void readline(char* buf, int max) {
    int p = 0;
    while (1) {
        char ch;
        if (readf(0, &ch, 1) <= 0) continue;   /* raw 模式: 无输入则继续等 */
        if (ch == 10 || ch == 13) break;
        if (ch == 27) {
            /* 只丢弃 ESC 本身, 不连带消费后续字节, 避免吞掉命令首字符 */
            continue;
        }
        if (ch == 127 || ch == 8) {
            if (p > 0) { p--; print("\b \b"); }
        } else if (p < max - 1 && ch >= 32 && ch < 127) {
            buf[p++] = ch;
            char tmp[2] = {ch, 0};
            print(tmp);
        }
    }
    buf[p] = '\0';
    print("\n");
}

// 拆分命令行为 argv, 返回 argc
static int tokenize(char* line, char** argv, int max_arg) {
    int argc = 0;
    char* p = line;
    while (*p && argc < max_arg - 1) {
        while (*p == ' ') p++;
        if (*p == 0) break;
        argv[argc++] = p;
        while (*p && *p != ' ') p++;
        if (*p == ' ') { *p = 0; p++; }
    }
    argv[argc] = 0;
    return argc;
}

static void usage(void) {
    print("  cd [dir]     - change directory\n");
    print("  pwd          - print working directory\n");
    print("  echo <msg>   - print message\n");
    print("  ls [path]    - list directory\n");
    print("  cat <file>   - print file content\n");
    print("  mkdir <dir>  - create directory\n");
    print("  touch <file> - create empty file\n");
    print("  rm <file>    - delete file\n");
    print("  clear        - clear screen\n");
    print("  editor <file> - edit text file (vim-like)\n");
    print("  spawn <prog> [args...] - run executable\n");
    print("  exit         - quit shell\n");
}

int main(int argc, char* argv[]) {
    str_cpy("/", cwd);
    print("\n=== BLKRv Shell ===\n");
    print("Commands: cd, pwd, echo, ls, cat, mkdir, touch, rm, clear, editor, spawn, exit\n");
    print("Type 'help' for details\n");

    char line[128];
    char* args[8];

    while (1) {
        print(">>> ");
        readline(line, 128);
        if (line[0] == 0) continue;

        int ac = tokenize(line, args, 8);
        if (ac == 0) continue;
        char* cmd = args[0];

        if (str_cmp(cmd, "exit")) { print("Goodbye!\n"); break; }
        else if (str_cmp(cmd, "help")) { usage(); }
        else if (str_cmp(cmd, "pwd")) { print(cwd); print("\n"); }
        else if (str_cmp(cmd, "cd")) {
            const char* target = (ac > 1) ? args[1] : "/";
            char abs[MAX_PATH];
            resolve_path(target, abs);
            int fd = openf(abs);
            if (fd < 0) { print("cd: no such directory\n"); continue; }
            inode_t ino;
            finfo(fd, &ino);
            closef(fd);   // cd 用完目录 fd 立即释放, 防止 fd 泄漏耗尽
            if (ino.type != 'd') { print("cd: not a directory\n"); continue; }
            str_cpy(abs, cwd);
        }
        else {
            // 独立程序: /bin/<cmd>
            char prog[MAX_PATH];
            str_cpy("/bin/", prog);
            str_cat(prog, cmd);
            int fd = openf(prog);
            if (fd < 0) { print("unknown: "); print(cmd); print("\n"); continue; }

            char* child_args[8];
            char resolved[8][MAX_PATH];
            int ca = 0;
            child_args[ca++] = prog;
            int ri = 0;
            // 路径类命令: 参数按 cwd 解析为绝对路径
            int path_cmd = str_cmp(cmd,"ls")||str_cmp(cmd,"cat")||
                           str_cmp(cmd,"mkdir")||str_cmp(cmd,"touch")||
                           str_cmp(cmd,"rm")||str_cmp(cmd,"spawn")||
                           str_cmp(cmd,"editor");
            if (str_cmp(cmd, "ls") && ac == 1) {
                // ls 无参数时列出当前目录
                resolve_path(cwd, resolved[ri]);
                child_args[ca++] = resolved[ri++];
            }
            for (int i = 1; i < ac && ca < 7; i++) {
                if (path_cmd) { resolve_path(args[i], resolved[ri]); child_args[ca++] = resolved[ri++]; }
                else          { child_args[ca++] = args[i]; }
            }
            child_args[ca] = 0;

            int pid = spawn(fd, child_args, ca);
            closef(fd);   // spawn 只用到 inode, 用完立即释放, 防止 fd 泄漏耗尽
            if (pid < 0) { print("spawn failed\n"); continue; }
            waitpid(pid);  // 前台命令
        }
    }
    return 0;
}
