#ifndef _DRIVERS_H_
#define _DRIVERS_H_
#include "syscall.h"
#include "mm.h"
#include "math.h"

char out_cache[IO_CACHE];
int out_cache_mutex=0;
char in_cache[IO_CACHE];
int in_cache_mutex=0;
int in_cache_frontp=0,in_cache_backp=0;

int vprint_i(char* str,uint32_t length);

int vgetch_i();

#endif // !_DRIVERS_H_
