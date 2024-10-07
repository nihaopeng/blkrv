#ifndef _DT_H_
#define _DT_H_

#include "ini.h"
typedef struct syscall_table
{
    uint32_t addr;
}desc_table[256];

desc_table syscall_table;
#endif // !_DT_H_



