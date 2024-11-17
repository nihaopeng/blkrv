#ifndef _DT_H_
#define _DT_H_

#include "ini.h"
#include "drivers.h"
#include "file.h"
typedef struct syscall_table
{
    uint32_t addr;
}desc_table[256];

void init_std();
void init_fs();
void init_proc();
// extern desc_table syscall_table,inter_table;
#endif // !_DT_H_



