#include "mm.h"

int free_i(void* pointer){
    uint32_t satp=0;
    __asm__ volatile("csrr %0,0x181\n"::"r"(satp));//获取satp的值，也就是页表基址
    return freek(satp&0xfffff000,pointer);
}

void* malloc_i(uint32_t size){
    uint32_t satp=0;
    __asm__ volatile("csrr %0,0x181\n"::"r"(satp));//获取satp的值，也就是页表基址
    return mallock(size,satp&0xfffff000);
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