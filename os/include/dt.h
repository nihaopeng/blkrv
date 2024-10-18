#ifndef _DT_H_
#define _DT_H_

#include "ini.h"
#include "drivers.h"
typedef struct syscall_table
{
    uint32_t addr;
}desc_table[256];

void init_std();
// extern desc_table syscall_table,inter_table;
#endif // !_DT_H_



