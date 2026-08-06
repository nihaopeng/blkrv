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










int poweroff_i();

int powoff();

int inputk(const char* fmt,...);

void keydown_interrupt_i();


void regist_keydown_interrupt(int* dt_addr);

void regist_poweroff(int* dt_addr);



void regist_powoff(int* dt_addr);

#endif // !_DRIVERS_H_
