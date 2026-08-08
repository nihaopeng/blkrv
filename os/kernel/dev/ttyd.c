#include "drivers.h"

// 纯字符流 TTY 通道: ttyd 只把输出字节转发给软件端 terminal 模拟器
// (terminal.c), 由它解析转义序列、维护主/备缓冲并驱动 screen 设备。
// 屏幕状态不再属于进程退出路径, exit 无需任何恢复逻辑。

// 纯 raw 模式 TTY 输入: 直接把键盘环形缓冲里已有的字符拷给用户
// 返回实际读取的字节数; 没有输入时返回 0 (非阻塞)
int tty_readk(char* buf,uint32_t count){
    uint32_t n=0;
    while(n<count){
        char ch=vgetchk();
        if(ch==0) break;
        buf[n++]=ch;
    }
    return n;
}

// TTY 输出: 把字节流整体交给 terminal 模拟器。
// \n / \r / \b / 转义序列等语义全部由 terminal.c 处理。
int tty_writek(const char* buf,uint32_t count){
    terminal_write(buf, count);
    return count;
}

// 兼容保留的整帧 API: 新架构下程序改用转义序列输出, 这里只返回终端尺寸
int tty_frame_i(const char* buf,uint32_t count,uint32_t row,uint32_t col){
    _vir2phyk(const char*,buf);
    (void)count; (void)row; (void)col;
    uint32_t w = *(uint32_t*)SCREEN_WIDTH_ADDR;
    uint32_t h = *(uint32_t*)SCREEN_HEIGHT_ADDR;
    return (int)((w<<16)|h);
}

_regist_syscall(void, tty_frame);

// 查询终端尺寸: 写回 W/H
int tty_size_i(uint32_t* w, uint32_t* h){
    _vir2phyk(uint32_t*,w);
    _vir2phyk(uint32_t*,h);
    *w = *(uint32_t*)SCREEN_WIDTH_ADDR;
    *h = *(uint32_t*)SCREEN_HEIGHT_ADDR;
    return 0;
}

_regist_syscall(void, tty_size);
