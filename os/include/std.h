#ifndef _STD_H_
#define _STD_H_

#include "ini.h"
#include "str.h"
#include "set_gate.h"
#include "syscall.h"
#include "dt.h"

int vprint(char* str,uint32_t length);
char vgetch();

#define get_va(n,va) \
__asm__ volatile( \
    "add a1,s0,%1\n" \
    "lw %0,0(a1)\n"\
    :"=r"(va) \
    :"r"(n) \
);

int print(const char* fmt,...);

int input(const char* fmt,...);

#endif // !_STD_H_


