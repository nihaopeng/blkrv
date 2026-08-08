#ifndef _BLKRV_H_
#define _BLKRV_H_

#include <stdint.h>

// ---------------- 基础常量 (与 os/include/ini.h 保持一致) ----------------
#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

#define FILE_TYPE 'f'
#define DIR_TYPE  'd'
#define MAX_NAME  128
#define MAX_PATH_LEN 256

// ---------------- 用户态可见的结构体 (与内核布局一致) ----------------
typedef struct file {
    char file_name[MAX_NAME];
    uint32_t size;
    uint32_t start_block;
    uint8_t type;
} inode;

typedef struct socket {
    uint32_t origin_port;
    char ip[16];
    uint32_t target_port;
} socket;

typedef struct point {
    int x;
    int y;
} point;

typedef struct color {
    int red;
    int green;
    int blue;
    int alpha;
} color;

// ---------------- 标准输出 (兼容旧 print) ----------------
int print(const char* fmt, ...);
int input(const char* fmt, ...);
int getline(char* str);

// ---------------- 文件系统 ----------------
int open(const char* path);
int close(int fd);
int read(int fd, void* buf, uint32_t count);
int write(int fd, const void* buf, uint32_t count);
int create(char* path, char type, void* inode_id);
int delete(int fd);
int finfo(int fd, void* inode_out);
int finoid(uint32_t inode_id, void* inode_out);

// ---------------- 进程 ----------------
int spawn(uint32_t inode_id, char** argv, uint32_t argc);
int waitpid(int pid);
int exec(uint32_t inode_id, int stdout_fd, char** para, uint32_t para_num);
void exit(int code);
void* malloc(uint32_t size);
void free(void* pointer);
int atoi(const char* str);

// ---------------- 电源 ----------------
void powoff(void);
int shutdown(void);

// ---------------- 网络 ----------------
int send(int sockfd, void* buf, uint32_t buf_length);
int recv(int sockfd, void* buf, uint32_t buf_length);
int accept(socket* sock);
int connect(socket* sock);

// ---------------- 图形 ----------------
int draw_label(point* p, char* str, color* c, int font);
int draw_triangle(point* p1, point* p2, point* p3, color* c);
int draw_jpg(uint32_t inode_id, uint32_t size, uint32_t x, uint32_t y);
int draw_png(uint32_t inode_id, uint32_t size, uint32_t x, uint32_t y);
int flush(void);

// ---------------- 监视器 / 终端 ----------------
int open_monitor(void);
int close_monitor(void);
int tty_frame(char* buf, uint32_t count, uint32_t row, uint32_t col);
int tty_size(unsigned* w, unsigned* h);
int flush_input(void);

// ---------------- 字符串兼容层 ----------------
uint32_t str_len(const char* str);
int str_cmp(const char* a, const char* b);        // 相同返回 1, 不同返回 0
void str_cpy(const char* src, char* dst);
void str_cat(char* dst, const char* src);
void str_cpy_s(const char* src, char* dst, uint32_t start, uint32_t end);
void split(const char* str, char separator, uint32_t* node);
void itoa(int num, char* str);
void xtoa(int num, char* str);
void ftoa(float num, char* str, int precision);
void uint32_to_char(uint32_t value, char* buf);
int memset_s(char* addr, char ch, uint32_t count);

#endif // !_BLKRV_H_
