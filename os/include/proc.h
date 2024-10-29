#ifndef _PROC_H_
#define _PROC_H_

typedef struct pcb{
    uint32_t pid;
    uint8_t status;
    uint32_t context_reg[32];
    uint32_t context_csr[32];
    uint32_t virtual_base_addr;
    int priority;
    int stdout;//0 for screen print, other is inode_id
    int stdout_start;
}pcb;



#endif // !_PROC_H_
