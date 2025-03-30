#include "drivers.h"

int poweroff_i(){
    __asm__ volatile(
        "li a0,%0\n"
        "sw zero,0(a0)"
        ::"i"(PMC_ADDR)
    );
}

_regist_syscall(void,poweroff);