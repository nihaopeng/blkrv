#ifndef _DRIVERS_H_
#define _DRIVERS_H_
#include "syscall.h"
#include "mm.h"
#include "math.h"
#include "dt.h"
#include "set_gate.h"

int vprint_i(char* str,uint32_t length);

int vgetch_i();

void regist_stdout();

void regist_stdin();

#endif // !_DRIVERS_H_
