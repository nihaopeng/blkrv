// BLKRv User Shell — 由调度器加载为 pid=1, 支持 spawn 外部程序

static void print(const char* s) {
    int n = 0; while (s[n]) n++;
    __asm__ volatile("li a7, 5\n mv a0, %0\n mv a1, %1\n ecall\n" :: "r"(s), "r"(n) : "a0","a1","a7");
}
static int openf(const char* path) {
    int r;
    __asm__ volatile("li a7, 0\n mv a0, %1\n ecall\n mv %0, a0" : "=r"(r) : "r"(path) : "a0","a7");
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
static int kbhit(void) {
    int r;
    __asm__ volatile("li a7, 8\n ecall\n mv %0, a0" : "=r"(r) :: "a0","a7");
    return r;
}
static char vgetch(void) {
    char r;
    __asm__ volatile("li a7, 6\n ecall\n mv %0, a0" : "=r"(r) :: "a0","a7");
    return r;
}

static int str_len(const char* s) { int n=0; while(s[n])n++; return n; }
static int str_cmp(const char* a, const char* b) {
    int la=str_len(a), lb=str_len(b);
    if(la!=lb) return 0;
    for(int i=0;i<la;i++) if(a[i]!=b[i]) return 0;
    return 1;
}

static void readline(char* buf, int max) {
    int p = 0;
    while (1) {
        while (!kbhit()) { /* spin */ }
        char ch = vgetch();
        if (ch == 0) continue;
        if (ch == 10 || ch == 13) break;
        if (ch == 127 || ch == 8) {
            if (p > 0) { p--; print("\b \b"); }
        } else if (p < max - 1) {
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
    while (*p && argc < max_arg) {
        while (*p == ' ') p++;
        if (*p == 0) break;
        argv[argc++] = p;
        while (*p && *p != ' ') p++;
        if (*p == ' ') { *p = 0; p++; }
    }
    argv[argc] = 0;
    return argc;
}

int main(int argc, char* argv[]) {
    print("\n=== BLKRv Shell ===\n");
    print("Commands: help, echo <msg>, ls, cat <f>, spawn <prog>, exit\n");

    char line[128];
    char* args[8];

    while (1) {
        print("> ");
        readline(line, 128);
        if (line[0] == 0) continue;

        int ac = tokenize(line, args, 8);
        if (ac == 0) continue;
        char* cmd = args[0];

        if (str_cmp(cmd, "exit")) { print("Goodbye!\n"); break; }
        else if (str_cmp(cmd, "help")) {
            print("  echo <msg>  - print message\n");
            print("  ls [path]   - list directory\n");
            print("  cat <file>  - print file content\n");
            print("  spawn <prog> [args...] - run executable\n");
            print("  exit        - quit shell\n");
        }
        else if (str_cmp(cmd, "echo")) {
            if (ac > 1) { print(args[1]); print("\n"); }
            else print("\n");
        }
        else if (str_cmp(cmd, "spawn") || str_cmp(cmd, "ls") || str_cmp(cmd, "cat")) {
            // 构造路径
            char* prog;
            if (str_cmp(cmd, "ls"))      prog = "/bin/ls";
            else if (str_cmp(cmd, "cat")) prog = "/bin/cat";
            else if (ac > 1)             prog = args[1];
            else { print("Usage: spawn <path> [args...]\n"); continue; }

            int fd = openf(prog);
            if (fd < 0) { print("open failed: "); print(prog); print("\n"); continue; }

            // 传参给子进程: argv[0]=程序名, 后面是剩余参数
            char* child_args[8];
            int ca = 0;
            child_args[ca++] = prog;
            int start = str_cmp(cmd, "spawn") ? 1 : 2;  // 跳过命令名
            for (; start < ac && ca < 7; start++)
                child_args[ca++] = args[start];
            child_args[ca] = 0;

            int pid = spawn(fd, child_args, ca);
            if (pid < 0) { print("spawn failed\n"); continue; }
            // 前台命令: 等待子进程执行完毕再继续
            waitpid(pid);
        }
        else {
            print("unknown: "); print(cmd); print("\n");
        }
    }
    return 0;
}
