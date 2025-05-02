#include "proc.h"

extern pcb global_pcb_list[MAX_PRO_NUM];

int free_i(void* pointer){
    uint32_t satp=0;
    __asm__ volatile("csrr %0,0x181\n"::"r"(satp));//获取satp的值，也就是页表基址
    mnode* free_block_head=&(global_pcb_list[satp&0x00000fff].free_block_head);
    return freek(pointer,(uint32_t*)(satp&0xfffff000),free_block_head);
}

void* malloc_i(uint32_t size){
    uint32_t satp=0;
    __asm__ volatile("csrr %0,0x181"::"r"(satp));//获取satp的值，也就是页表基址
    // printk("malloc satp:%x\n",satp);
    mnode* free_block_head=&(global_pcb_list[satp&0x00000fff].free_block_head);
    return mallock(size,(uint32_t*)(satp&0xfffff000),free_block_head);
}

_regist_syscall(void,free);

_regist_syscall(void,malloc);

int memset_s(char* addr,char ch,uint32_t count){
    for(int i=0;i<count;i++){
        *(addr) = ch;
        addr+=1;
    }
    return 0;
}