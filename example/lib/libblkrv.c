#include "blkrv.h"
#include <stdarg.h>

// ============================================================
//  BLKRv 用户态库: 所有 ecall 的封装集中在这里.
//  程序只调用 blkrv.h 里的函数, 不再自己写内联汇编.
//  自实现 printf 风格格式化与字符串函数 (语义与 os/kernel/utils/str.c 一致),
//  不依赖 newlib / libc, 二进制更小.
//  链接时仅需要 -lgcc (提供 __divsi3/__mulsi3 等软指令例程).
// ============================================================

// ---------------- 文件系统 ----------------
int open(const char* path) {
    int r;
    __asm__ volatile("li a7, 0\n mv a0, %1\n ecall\n mv %0, a0"
        : "=r"(r) : "r"(path) : "a0","a7");
    return r;
}

int close(int fd) {
    int r;
    __asm__ volatile("li a7, 25\n mv a0, %1\n ecall\n mv %0, a0"
        : "=r"(r) : "r"(fd) : "a0","a7");
    return r;
}

int read(int fd, void* buf, uint32_t count) {
    int r;
    __asm__ volatile("li a7, 3\n mv a0, %1\n mv a1, %2\n mv a2, %3\n ecall\n mv %0, a0"
        : "=r"(r) : "r"(fd), "r"(buf), "r"(count) : "a0","a1","a2","a7");
    return r;
}

int write(int fd, const void* buf, uint32_t count) {
    int r;
    __asm__ volatile("li a7, 2\n mv a0, %1\n mv a1, %2\n mv a2, %3\n ecall\n mv %0, a0"
        : "=r"(r) : "r"(fd), "r"(buf), "r"(count) : "a0","a1","a2","a7");
    return r;
}

int create(char* path, char type, void* inode_id) {
    int r;
    __asm__ volatile("li a7, 4\n mv a0, %1\n mv a1, %2\n mv a2, %3\n ecall\n mv %0, a0"
        : "=r"(r) : "r"(path), "r"(type), "r"(inode_id) : "a0","a1","a2","a7");
    return r;
}

int delete(int fd) {
    int r;
    __asm__ volatile("li a7, 1\n mv a0, %1\n ecall\n mv %0, a0"
        : "=r"(r) : "r"(fd) : "a0","a7");
    return r;
}

int finfo(int fd, void* inode_out) {
    int r;
    __asm__ volatile("li a7, 20\n mv a0, %1\n mv a1, %2\n ecall\n mv %0, a0"
        : "=r"(r) : "r"(fd), "r"(inode_out) : "a0","a1","a7");
    return r;
}

int finoid(uint32_t inode_id, void* inode_out) {
    int r;
    __asm__ volatile("li a7, 28\n mv a0, %1\n mv a1, %2\n ecall\n mv %0, a0"
        : "=r"(r) : "r"(inode_id), "r"(inode_out) : "a0","a1","a7");
    return r;
}

// ---------------- 进程 ----------------
int spawn(uint32_t inode_id, char** argv, uint32_t argc) {
    int r;
    __asm__ volatile("li a7, 26\n mv a0, %1\n mv a1, %2\n mv a2, %3\n ecall\n mv %0, a0"
        : "=r"(r) : "r"(inode_id), "r"(argv), "r"(argc) : "a0","a1","a2","a7");
    return r;
}

int waitpid(int pid) {
    int r;
    __asm__ volatile("li a7, 27\n mv a0, %1\n ecall\n mv %0, a0"
        : "=r"(r) : "r"(pid) : "a0","a7");
    return r;
}

int exec(uint32_t inode_id, int stdout_fd, char** para, uint32_t para_num) {
    int r;
    __asm__ volatile("li a7, 10\n mv a0, %1\n mv a1, %2\n mv a2, %3\n mv a3, %4\n ecall\n mv %0, a0"
        : "=r"(r) : "r"(inode_id), "r"(stdout_fd), "r"(para), "r"(para_num)
        : "a0","a1","a2","a3","a7");
    return r;
}

void exit(int code) {
    (void)code;
    __asm__ volatile("li a7, 9\n ecall\n");
    for(;;);
}

void* malloc(uint32_t size) {
    void* r;
    __asm__ volatile("li a7, 18\n mv a0, %1\n ecall\n mv %0, a0"
        : "=r"(r) : "r"(size) : "a0","a7");
    return r;
}

void free(void* pointer) {
    __asm__ volatile("li a7, 19\n mv a0, %0\n ecall\n" :: "r"(pointer) : "a0","a7");
}

// ---------------- 电源 ----------------
void powoff(void) {
    __asm__ volatile("li a7, 7\n ecall\n");
    for(;;);
}

int shutdown(void) {
    print("\n---!powoff now!---\n");
    powoff();
    return 0;
}

// ---------------- 网络 ----------------
int send(int sockfd, void* buf, uint32_t buf_length) {
    int r;
    __asm__ volatile("li a7, 11\n mv a0, %1\n mv a1, %2\n mv a2, %3\n ecall\n mv %0, a0"
        : "=r"(r) : "r"(sockfd), "r"(buf), "r"(buf_length) : "a0","a1","a2","a7");
    return r;
}

int recv(int sockfd, void* buf, uint32_t buf_length) {
    int r;
    __asm__ volatile("li a7, 12\n mv a0, %1\n mv a1, %2\n mv a2, %3\n ecall\n mv %0, a0"
        : "=r"(r) : "r"(sockfd), "r"(buf), "r"(buf_length) : "a0","a1","a2","a7");
    return r;
}

int accept(socket* sock) {
    int r;
    __asm__ volatile("li a7, 23\n mv a0, %1\n ecall\n mv %0, a0"
        : "=r"(r) : "r"(sock) : "a0","a7");
    return r;
}

int connect(socket* sock) {
    int r;
    __asm__ volatile("li a7, 24\n mv a0, %1\n ecall\n mv %0, a0"
        : "=r"(r) : "r"(sock) : "a0","a7");
    return r;
}

// ---------------- 图形 ----------------
int draw_label(point* p, char* str, color* c, int font) {
    int r;
    __asm__ volatile("li a7, 13\n mv a0, %1\n mv a1, %2\n mv a2, %3\n mv a3, %4\n ecall\n mv %0, a0"
        : "=r"(r) : "r"(p), "r"(str), "r"(c), "r"(font) : "a0","a1","a2","a3","a7");
    return r;
}

int draw_triangle(point* p1, point* p2, point* p3, color* c) {
    int r;
    __asm__ volatile("li a7, 14\n mv a0, %1\n mv a1, %2\n mv a2, %3\n mv a3, %4\n ecall\n mv %0, a0"
        : "=r"(r) : "r"(p1), "r"(p2), "r"(p3), "r"(c) : "a0","a1","a2","a3","a7");
    return r;
}

int draw_jpg(uint32_t inode_id, uint32_t size, uint32_t x, uint32_t y) {
    int r;
    __asm__ volatile("li a7, 21\n mv a0, %1\n mv a1, %2\n mv a2, %3\n mv a3, %4\n ecall\n mv %0, a0"
        : "=r"(r) : "r"(inode_id), "r"(size), "r"(x), "r"(y) : "a0","a1","a2","a3","a7");
    return r;
}

int draw_png(uint32_t inode_id, uint32_t size, uint32_t x, uint32_t y) {
    int r;
    __asm__ volatile("li a7, 22\n mv a0, %1\n mv a1, %2\n mv a2, %3\n mv a3, %4\n ecall\n mv %0, a0"
        : "=r"(r) : "r"(inode_id), "r"(size), "r"(x), "r"(y) : "a0","a1","a2","a3","a7");
    return r;
}

int flush(void) {
    int r;
    __asm__ volatile("li a7, 15\n ecall\n mv %0, a0" : "=r"(r) : : "a0","a7");
    return r;
}

// ---------------- 监视器 / 终端 ----------------
int open_monitor(void) {
    int r;
    __asm__ volatile("li a7, 16\n ecall\n mv %0, a0" : "=r"(r) : : "a0","a7");
    return r;
}

int close_monitor(void) {
    int r;
    __asm__ volatile("li a7, 17\n ecall\n mv %0, a0" : "=r"(r) : : "a0","a7");
    return r;
}

int tty_frame(char* buf, uint32_t count, uint32_t row, uint32_t col) {
    int r;
    __asm__ volatile("li a7, 29\n mv a0, %1\n mv a1, %2\n mv a2, %3\n mv a3, %4\n ecall\n mv %0, a0"
        : "=r"(r) : "r"(buf), "r"(count), "r"(row), "r"(col) : "a0","a1","a2","a3","a7");
    return r;
}

int tty_size(unsigned* w, unsigned* h) {
    int r;
    __asm__ volatile("li a7, 30\n mv a0, %1\n mv a1, %2\n ecall\n mv %0, a0"
        : "=r"(r) : "r"(w), "r"(h) : "a0","a1","a7");
    return r;
}

int flush_input(void) {
    int r;
    __asm__ volatile("li a7, 31\n ecall\n mv %0, a0" : "=r"(r) : : "a0","a7");
    return r;
}

// ---------------- 字符串兼容层 (语义与 os/kernel/utils/str.c 一致) ----------------
uint32_t str_len(const char* str) {
    uint32_t i = 0;
    while (str[i] != '\0') i++;
    return i;
}

// 相同返回 1, 不同返回 0 (注意: 不是标准 strcmp!)
int str_cmp(const char* a, const char* b) {
    if (str_len(a) != str_len(b)) return 0;
    uint32_t i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return 0;
        i++;
    }
    return 1;
}

void str_cpy(const char* src, char* dst) {
    uint32_t data_len = str_len(src);
    for (uint32_t i = 0; i < data_len; i++) dst[i] = src[i];
    dst[data_len] = '\0';
}

void str_cat(char* dst, const char* src) {
    int i = (int)str_len(dst), j = 0;
    while (src[j]) { dst[i++] = src[j++]; }
    dst[i] = 0;
}

// 复制 src[start..end] (闭区间), 不添加终止符
void str_cpy_s(const char* src, char* dst, uint32_t start, uint32_t end) {
    for (uint32_t i = start; i <= end; i++) dst[i] = src[i];
}

void split(const char* str, char separator, uint32_t* node) {
    uint32_t length = str_len(str);
    uint32_t pos = 0;
    for (uint32_t i = 0; i < length; i++) {
        if (str[i] == separator) node[pos++] = i;
    }
}

void itoa(int num, char* str) {
    if (num == 0) { str[0] = '0'; str[1] = '\0'; return; }
    int is_negative = num < 0;
    // 用无符号处理, 避免 INT_MIN 取反溢出
    uint32_t n = is_negative ? (uint32_t)(-(num + 1)) + 1 : (uint32_t)num;
    int pos = 0;
    while (n != 0) {
        str[pos++] = (char)('0' + (n % 10));
        n /= 10;
    }
    if (is_negative) str[pos++] = '-';
    str[pos] = '\0';
    for (int i = 0; i < pos / 2; i++) {
        char temp = str[i];
        str[i] = str[pos - i - 1];
        str[pos - i - 1] = temp;
    }
}

void xtoa(int num, char* str) {
    char buffer[17];
    int is_negative = 0;
    uint32_t unum;
    if (num == 0) { str[0] = '0'; str[1] = '\0'; return; }
    int n = num;
    if (n < 0) { is_negative = 1; unum = (uint32_t)(-n); }
    else unum = (uint32_t)n;
    int index = sizeof(buffer) - 1;
    buffer[index--] = '\0';
    const char digits[] = "0123456789ABCDEF";
    do {
        buffer[index--] = digits[unum % 16];
        unum /= 16;
    } while (unum > 0);
    if (is_negative) buffer[index--] = '-';
    char* src = buffer + index + 1;
    while ((*str++ = *src++) != '\0');
}

void ftoa(float num, char* buffer, int precision) {
    int is_negative = 0;
    if (num < 0) { is_negative = 1; num = -num; }
    int integer_part = (int)num;
    float decimal_part = num - (float)integer_part;
    char int_buffer[16];
    int i = 0;
    do {
        int_buffer[i++] = (char)('0' + (integer_part % 10));
        integer_part /= 10;
    } while (integer_part > 0);
    int j = 0;
    if (is_negative) buffer[j++] = '-';
    while (i > 0) buffer[j++] = int_buffer[--i];
    buffer[j++] = '.';
    for (int k = 0; k < precision; k++) {
        decimal_part *= 10;
        int digit = (int)decimal_part;
        buffer[j++] = (char)('0' + digit);
        decimal_part -= digit;
    }
    buffer[j] = '\0';
}

int atoi(const char* str) {
    int res = 0;
    int sign = 1;
    while (*str == 32) str++;
    if (*str == '+' || *str == '-') {
        sign = (*str == '-') ? -1 : 1;
        str++;
    }
    while (*str >= '0' && *str <= '9') {
        res = res * 10 + *str - '0';
        str++;
    }
    return sign * res;
}

void uint32_to_char(uint32_t value, char* buf) {
    buf[0] = (char)(value & 0xFF);
    buf[1] = (char)((value >> 8) & 0xFF);
    buf[2] = (char)((value >> 16) & 0xFF);
    buf[3] = (char)((value >> 24) & 0xFF);
}

int memset_s(char* addr, char ch, uint32_t count) {
    for (uint32_t i = 0; i < count; i++) {
        *addr = ch;
        addr += 1;
    }
    return 0;
}

// 编译器可能为结构体赋值/大拷贝生成 memcpy/memset, 提供最小实现避免依赖 libc
void* memcpy(void* dst, const void* src, uint32_t n) {
    char* d = (char*)dst;
    const char* s = (const char*)src;
    for (uint32_t i = 0; i < n; i++) d[i] = s[i];
    return dst;
}

void* memset(void* dst, int c, uint32_t n) {
    char* d = (char*)dst;
    for (uint32_t i = 0; i < n; i++) d[i] = (char)c;
    return dst;
}

// ---------------- 标准输出 (print/input/getline) ----------------
#define PRINT_BUF_SIZE 1024

static void out_flush(char* out, uint32_t* n) {
    if (*n > 0) {
        write(STDOUT_FILENO, out, *n);
        *n = 0;
    }
}

static void out_put(char* out, uint32_t* n, const char* s, uint32_t len) {
    if (len >= PRINT_BUF_SIZE) {
        out_flush(out, n);
        write(STDOUT_FILENO, s, len);
        return;
    }
    if (*n + len > PRINT_BUF_SIZE) out_flush(out, n);
    for (uint32_t i = 0; i < len; i++) out[(*n)++] = s[i];
}

int print(const char* fmt, ...) {
    static char out[PRINT_BUF_SIZE];
    uint32_t n = 0;
    va_list args;
    va_start(args, fmt);
    for (uint32_t i = 0; fmt[i] != '\0'; i++) {
        if (fmt[i] == '%' && fmt[i + 1] != '\0') {
            i++;
            char tmp[40];
            const char* s;
            switch (fmt[i]) {
                case 'c': {
                    char ch = (char)va_arg(args, int);
                    out_put(out, &n, &ch, 1);
                    break;
                }
                case 's':
                    s = va_arg(args, const char*);
                    if (s == 0) s = "(null)";
                    out_put(out, &n, s, str_len(s));
                    break;
                case 'd':
                    itoa(va_arg(args, int), tmp);
                    out_put(out, &n, tmp, str_len(tmp));
                    break;
                case 'x':
                    xtoa(va_arg(args, int), tmp);
                    out_put(out, &n, tmp, str_len(tmp));
                    break;
                case 'f': {
                    float f = (float)va_arg(args, double);
                    ftoa(f, tmp, 6);
                    out_put(out, &n, tmp, str_len(tmp));
                    break;
                }
                case '%':
                    out_put(out, &n, "%", 1);
                    break;
                default:
                    out_put(out, &n, "%", 1);
                    out_put(out, &n, fmt + i, 1);
                    break;
            }
        } else {
            out_put(out, &n, fmt + i, 1);
        }
    }
    va_end(args);
    out_flush(out, &n);
    return (int)n;
}

int input(const char* fmt, ...) {
    int fmt_len = (int)str_len(fmt);
    va_list args;
    va_start(args, fmt);
    for (int i = 0; i < fmt_len; i++) {
        if (fmt[i] == '%') {
            char fmts[64];
            int p = 0;
            while (1) {
                char ch;
                if (read(STDIN_FILENO, &ch, 1) <= 0) continue;
                if (ch == 10 || ch == 32) {
                    if (p == 0) continue;
                    else break;
                } else if (ch != 0) {
                    if (ch == 127) { if (p > 0) p -= 1; fmts[p] = 0; }
                    else fmts[p++] = ch;
                }
            }
            switch (fmt[i + 1]) {
                case 'c': {
                    char* ch = va_arg(args, char*);
                    *ch = fmts[0];
                    break;
                }
                case 's': {
                    char* str = va_arg(args, char*);
                    str_cpy_s(fmts, str, 0, (uint32_t)(p - 1));
                    str[p] = 0;
                    break;
                }
                case 'd': {
                    int* va = va_arg(args, int*);
                    char tmp[12];
                    str_cpy_s(fmts, tmp, 0, (uint32_t)(p - 1));
                    tmp[p] = 0;
                    *va = atoi(tmp);
                    break;
                }
                default:
                    break;
            }
            i++;
        }
    }
    va_end(args);
    return 0;
}

int getline(char* str) {
    uint32_t p = 0;
    while (1) {
        char ch;
        if (read(STDIN_FILENO, &ch, 1) <= 0) continue;
        if (ch == 10) break;
        else if (ch != 0) {
            if (ch == 127) { if (p > 0) p -= 1; str[p] = 0; }
            else str[p++] = ch;
        }
    }
    str[p] = 0;
    return (int)p;
}
