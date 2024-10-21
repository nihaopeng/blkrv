#include "drivers.h"

_syscall0(int,powoff);

int poweroff_i(){
    __asm__ volatile(
        "li a0,0x70000000\n"
        "sw zero,0(a0)"
        ::
    );
}

void regist_poweroff(int* dt_addr){
    int* func_addr_poweroff=(int*)(&poweroff_i);
    _set_gate(dt_addr,func_addr_poweroff);
}