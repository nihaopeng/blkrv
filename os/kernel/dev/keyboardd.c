#include "drivers.h"

int vgetch_i(char* ch){//change to syscall
    uint32_t p=0;
    __asm__ volatile(
        "csrr %0,0x181"
        :"=r"(p)
    );
    p=(p<<1)>>1;//去除mmu标志位
    ch=(char*)((void*)ch+p);
    vgetchk(ch);
}

int kbhit_i(int* ifhit){//change to syscall//kbhit ret 1
    uint32_t p=0;
    __asm__ volatile(
        "csrr %0,0x181"
        :"=r"(p)
    );
    p=(p<<1)>>1;//去除mmu标志位
    ifhit=(int*)((void*)ifhit+p);
    kbhitk(ifhit);
}

void regist_vgetch(int* dt_addr){
    int* func_addr_vgetch_i=(int*)(&vgetch_i);
    _set_gate(dt_addr,func_addr_vgetch_i);
}

void regist_kbhit(int* dt_addr){
    int* func_addr_kbhit_i=(int*)(&kbhit_i);
    _set_gate(dt_addr,func_addr_kbhit_i);
}