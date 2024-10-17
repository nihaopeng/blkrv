#ifndef _STD_H_
#define _STD_H_

#include "ini.h"
#include "str.h"
#include "set_gate.h"
#include "syscall.h"
#include "drivers.h"

extern char out_cache[IO_CACHE];
extern int out_cache_mutex;
extern char in_cache[IO_CACHE];
extern int in_cache_mutex;
extern int in_cache_frontp;
extern int in_cache_backp;

_syscall2(int,vprint,char*,str,uint32_t,length);
_syscall0(char,vgetch);

#define get_va(n,va) \
__asm__ volatile( \
    "add a1,s0,%1\n" \
    "lw %0,0(a1)\n"\
    :"=r"(va) \
    :"r"(n) \
);

int print(const char* fmt,...);

int input(const char* fmt,...);

void init_std();

#endif // !_STD_H_


