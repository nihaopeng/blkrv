#ifndef _DRIVERS_H_
#define _DRIVERS_H_

#include "mm.h"
#include "math.h"
#include "set_gate.h"
#include "syscall.h"
#include "net.h"
#include "proc.h"

void set_stdout(int stdouts,int stdout_start);

int printk(const char* fmt,...);

int vprint_i(char* str,uint32_t length);

int vprint(char* str,uint32_t length);

int vgetch_i(char* ch);

int vgetch(char* ch);

int kbhit_i(int* ifhit);

int kbhit(int* ifhit);

int poweroff_i();

int powoff();

void regist_vprint(int* dt_addr);

void regist_keydown_int(int* dt_addr);

void regist_poweroff(int* dt_addr);

void regist_vgetch(int* dt_addr);

void regist_kbhit(int* dt_addr);

void regist_powoff(int* dt_addr);

#endif // !_DRIVERS_H_
