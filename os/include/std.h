#ifndef _STD_H_
#define _STD_H_

#include "ini.h"
#include "str.h"
#include "set_gate.h"
#include "syscall.h"
#include "drivers.h"

extern char out_cache[1024*1024];

#define get_va(n,va) \
__asm__ volatile( \
    "add a1,s0,%1\n" \
    "lw %0,0(a1)\n"\
    :"=r"(va) \
    :"r"(n) \
);

int print(const char* fmt,...);//only support 'c' now;

void init_std();

#endif // !_STD_H_


