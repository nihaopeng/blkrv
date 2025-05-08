#ifndef _DRIVERS_H_
#define _DRIVERS_H_

#include "ini.h"
#include "math.h"
#include "set_gate.h"
#include "syscall.h"


void set_stdout(int stdouts,int stdout_start);

int init_input();

int init_net();

int init_out();

int printk(const char* fmt,...);

int vprintk(char* str,uint32_t length);

int vprint_i(char* str,uint32_t length);

int vprint(char* str,uint32_t length);

char vgetchk();

char vgetch_i();

char vgetch();

int kbhitk();

int kbhit_i();

int kbhit();

int poweroff_i();

int powoff();

int inputk(const char* fmt,...);

void keydown_interrupt_i();

void regist_vprint(int* dt_addr);

void regist_keydown_interrupt(int* dt_addr);

void regist_poweroff(int* dt_addr);

void regist_vgetch(int* dt_addr);

void regist_kbhit(int* dt_addr);

void regist_powoff(int* dt_addr);

#endif // !_DRIVERS_H_
