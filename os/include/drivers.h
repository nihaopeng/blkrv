#ifndef _DRIVERS_H_
#define _DRIVERS_H_

#include "ini.h"
#include "math.h"
#include "set_gate.h"
#include "syscall.h"


int init_input();

int init_net();

int init_out();

int printk(const char* fmt,...);

char vgetchk();

int kbhitk();

int poweroff_i();

int powoff();

int inputk(const char* fmt,...);

// TTY 驱动: 用户态 read/write 在 fd=0/1/2 时最终落到这里
int tty_readk(char* buf,uint32_t count);

int tty_writek(const char* buf,uint32_t count);

// 软件端终端模拟器: 解析转义序列、维护主/备缓冲并驱动 screen 设备
void terminal_write(const char* buf,uint32_t count);

int tty_frame(char* buf,uint32_t count,uint32_t row,uint32_t col);

void regist_tty_frame(int* dt_addr);

int tty_size(uint32_t* w,uint32_t* h);

void regist_tty_size(int* dt_addr);

void keydown_interrupt_i();

void regist_keydown_interrupt(int* dt_addr);

int flush_input();

void regist_flush_input(int* dt_addr);

void regist_poweroff(int* dt_addr);

void regist_powoff(int* dt_addr);

void regist_timer_interrupt(int* dt_addr);

#endif // !_DRIVERS_H_
