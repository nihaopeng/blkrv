#ifndef _PROC_H_
#define _PROC_H_
#include "file.h"
#include "drivers.h"

typedef struct pcb{
    uint32_t pid;
    uint8_t status;
    uint32_t pc_reg;
    uint32_t context_reg[32];
    uint32_t context_csr[32];
    uint32_t virtual_base_addr;
    uint32_t heap_addr;
    int stdout;//0 for screen print, other is inode_id
    int stdout_start;
}pcb;

int init_ps();

void save_contxt(uint32_t* context_reg,uint32_t* context_csr,uint32_t pc_reg);

void enter_prog(uint32_t ram_start_addr);

int exit_i();

int exit();

int exec(uint32_t inode_id,int priority,int stdout,int stdout_start,int* pid,int* status,char* para[]);

int scheduler(int pid);

void regist_exit(int* gdt_addr_exit);

#endif // !_PROC_H_
