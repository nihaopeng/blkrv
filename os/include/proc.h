#ifndef _PROC_H_
#define _PROC_H_
#include "file.h"
#include "drivers.h"

// uint32_t front_pro_pid=1;

typedef struct pcb{
    uint8_t is_alive;
    uint32_t pc;
    uint32_t general_regs[32];
    uint32_t satp;
    free_vir_block free_vir_block_list;
    int stdout;//-1 for screen print, other is inode_id
}pcb;

int init_ps();

void save_contxt();

void recover_contxt();

void enter_prog();

int exit_i();

int exit();

int exec_i(uint32_t inode_id,int stdout,char** para,uint32_t para_num);

int exec(uint32_t inode_id,int stdout,char** para,uint32_t para_num);

int scheduler();

void regist_exec(int* gdt_addr_exec);

void regist_exit(int* gdt_addr_exit);

#endif // !_PROC_H_
