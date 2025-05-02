#ifndef _DT_H_
#define _DT_H_

#include "ini.h"
#include "drivers.h"
#include "file.h"
#include "proc.h"

typedef struct syscall_table
{
    uint32_t addr;
}desc_table[256];

void regist_sysmethod();
void regist_std();
void regist_fs();
void regist_ps();
void regist_net();
void regist_mm();
void regist_graphic();
void regist_monitor();

// extern desc_table syscall_table,inter_table;
#endif // !_DT_H_



