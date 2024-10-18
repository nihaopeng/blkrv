#ifndef _DRIVERS_H_
#define _DRIVERS_H_

#include "mm.h"
#include "math.h"
#include "set_gate.h"

int vprint_i(char* str,uint32_t length);

int vgetch_i();

void regist_stdout(int* dt_table_addr);

void regist_stdin(int* dt_table_addr);

void regist_keydown_int(int* dt_addr);

#endif // !_DRIVERS_H_
