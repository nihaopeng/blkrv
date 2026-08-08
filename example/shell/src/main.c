// BLKRv User Shell — pid=1
// 内建: cd / pwd / help / exit (cd 必须内建, 子进程无法改变父进程工作目录)
// 外部程序: echo / ls / cat / mkdir / touch / rm / clear (位于 /bin)

#include "blkrv.h"

#define MAX_PATH 256

static char cwd[MAX_PATH];

// ---------------- 历史记录 (上下箭头切换, 持久化到 /tmp/.history) ----------------
#define HIST_MAX 32
#define HIST_LEN 128

static char hist[HIST_MAX][HIST_LEN];
static int hist_n = 0;
static int hist_pos = 0;      // 当前浏览位置, hist_n 表示"空输入"

// 追加一条历史 (跳过空行, 相邻重复去重, 满 32 条后滚动丢弃最旧)
static void hist_add(const char* line) {
    if (line[0] == 0) return;
    hist_pos = hist_n;   // 无论是否去重, 执行完命令后浏览位置都回到末尾
    if (hist_n > 0 && str_cmp(hist[hist_n-1], line)) return;
    if (hist_n < HIST_MAX) {
        str_cpy(line, hist[hist_n]);
        hist_n++;
    } else {
        for (int i = 1; i < HIST_MAX; i++) str_cpy(hist[i-1], hist[i]);
        str_cpy(line, hist[HIST_MAX-1]);
    }
}

// 把全部历史写回 /tmp/.history (create 对已存在文件返回 offset 0 的新 fd,
// 顺序写完时 writek 会把 size 截断为新内容长度, 等价于覆写整个文件)
static void hist_save(void) {
    if (hist_n == 0) return;
    char path[] = "/tmp/.history";
    uint32_t ino;
    int fd = create(path, FILE_TYPE, &ino);
    if (fd < 0) return;
    for (int i = 0; i < hist_n; i++) {
        write(fd, hist[i], str_len(hist[i]));
        write(fd, "\n", 1);
    }
    close(fd);
}

// 启动时从历史文件恢复 (逐块读, 支持跨块断行)
static void hist_load(void) {
    char path[] = "/tmp/.history";
    int fd = open(path);
    if (fd < 0) return;
    char carry[HIST_LEN];
    int carry_n = 0;
    char rb[64];
    int n;
    while ((n = read(fd, rb, sizeof(rb))) > 0) {
        int start = 0;
        for (int i = 0; i < n; i++) {
            if (rb[i] == '\n') {
                char line[HIST_LEN];
                int len = 0;
                for (int c = 0; c < carry_n && len < HIST_LEN-1; c++) line[len++] = carry[c];
                for (int c = start; c < i && len < HIST_LEN-1; c++) line[len++] = rb[c];
                line[len] = 0;
                if (len > 0) hist_add(line);
                carry_n = 0;
                start = i + 1;
            }
        }
        carry_n = 0;
        for (int i = start; i < n && carry_n < HIST_LEN-1; i++) carry[carry_n++] = rb[i];
    }
    if (carry_n > 0) {
        carry[carry_n] = 0;
        hist_add(carry);
    }
    close(fd);
    hist_pos = hist_n;
}

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

static void readline(char* buf, int max, const char* prompt) {
    int p = 0;
    while (1) {
        char ch;
            if (read(0, &ch, 1) <= 0) continue;   /* raw 模式: 无输入则继续等 */
        if (ch == 10 || ch == 13) break;
        if (ch == 27) {
            /* 解析 ESC [ A / ESC [ B: 上下箭头切换历史; 其他 ESC 序列直接丢弃 */
            char seq[2] = {0, 0};
            int got = 0;
            for (int k = 0; k < 2; k++) {
                int r = 0, tries = 0;
                while (tries < 6 && r <= 0) {   // 同一按键的后续字节可能分拍到达, 短忙等
                    r = read(0, &seq[k], 1);
                    if (r <= 0) {
                        volatile int spin = 0;
                        while (spin < 4000) spin++;
                        tries++;
                    }
                }
                if (r <= 0) break;
                got++;
            }
            if (got == 2 && seq[0] == '[' && (seq[1] == 'A' || seq[1] == 'B')) {
                if (seq[1] == 'A' && hist_pos > 0) hist_pos--;
                else if (seq[1] == 'B' && hist_pos < hist_n) hist_pos++;
                print("\r\e[K");               // 回到行首清掉整行, 重绘 提示符+历史行
                print(prompt);
                p = 0;
                if (hist_pos < hist_n) {
                    str_cpy(hist[hist_pos], buf);
                    p = str_len(buf);
                    print(buf);
                } else {
                    buf[0] = 0;                // 按 Down 回到最末尾: 只保留提示符
                }
            }
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
    print("  ls [-l] [path] - list directory (-l: 详细)\n");
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
    hist_load();   // 从 /tmp/.history 恢复上次会话

    char line[128];
    char* args[8];

    while (1) {
        char prompt[MAX_PATH + 8];
        str_cpy(cwd, prompt);
        str_cat(prompt, " >>> ");
        print(prompt);
        readline(line, 128, prompt);
        if (line[0] == 0) continue;
        hist_add(line);   // 记入历史并立即持久化, 异常退出也不丢
        hist_save();

        int ac = tokenize(line, args, 8);
        if (ac == 0) continue;
        char* cmd = args[0];

        if (str_cmp(cmd, "exit")) { hist_save(); print("Goodbye!\n"); break; }
        else if (str_cmp(cmd, "help")) { usage(); }
        else if (str_cmp(cmd, "pwd")) { print(cwd); print("\n"); }
        else if (str_cmp(cmd, "cd")) {
            const char* target = (ac > 1) ? args[1] : "/";
            char abs[MAX_PATH];
            resolve_path(target, abs);
            int fd = open(abs);
            if (fd < 0) { print("cd: no such directory\n"); continue; }
            inode ino;
            finfo(fd, &ino);
            close(fd);   // cd 用完目录 fd 立即释放, 防止 fd 泄漏耗尽
            if (ino.type != 'd') { print("cd: not a directory\n"); continue; }
            str_cpy(abs, cwd);
        }
        else {
            // 独立程序: /bin/<cmd>
            char prog[MAX_PATH];
            str_cpy("/bin/", prog);
            str_cat(prog, cmd);
            int fd = open(prog);
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
                if (path_cmd && args[i][0] != '-') {
                    resolve_path(args[i], resolved[ri]);
                    child_args[ca++] = resolved[ri++];
                } else {
                    child_args[ca++] = args[i];   // 选项 (-l 等) 或普通参数原样传
                }
            }
            child_args[ca] = 0;

            int pid = spawn(fd, child_args, ca);
            close(fd);   // spawn 只用到 inode, 用完立即释放, 防止 fd 泄漏耗尽
            if (pid < 0) { print("spawn failed\n"); continue; }
            waitpid(pid);  // 前台命令
        }
    }
    return 0;
}
