#include "drivers.h"

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

// TTY 输出: 写到屏幕缓冲, 复用现有 SCREEN_CACHE1/CTRL 渲染路径
int tty_writek(const char* buf,uint32_t count){
    for(uint32_t i=0;i<count;i++){
        char* addr=(char*)SCREEN_CACHE1_ADDR+i;
        *addr=buf[i];
    }
    // 屏幕驱动扫描到 '\0' 才停止, 补一个结束符
    *(char*)(SCREEN_CACHE1_ADDR+count)='\0';
    char* ctrl_addr=(char*)SCREEN_CTRL_ADDR+3;
    *ctrl_addr=1;
    return count;
}
