#ifndef _STD_H_
#define _STD_H_

#include "ini.h"
#include "str.h"
#include "set_gate.h"
#include "syscall.h"
#include "dt.h"
#include "drivers.h"

#define get_va(n,va) \
__asm__ volatile( \
    "add a1,s0,%1\n" \
    "lw %0,0(a1)\n"\
    :"=r"(va) \
    :"r"(n) \
);

void set_stdout(int stdouts,int stdout_start);

int printk(const char* fmt,...);

int print(const char* fmt,...);

int input(const char* fmt,...);

int shutdown();

#endif // !_STD_H_


