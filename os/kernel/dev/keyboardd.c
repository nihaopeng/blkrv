#include "drivers.h"

int vgetch_i(char* ch){//change to syscall
    _vir2phy(char*,ch);
    vgetchk(ch);
}

int kbhit_i(int* ifhit){//change to syscall//kbhit ret 1
    _vir2phy(int*,ifhit);
    kbhitk(ifhit);
}

_regist_syscall(void,vgetch);

_regist_syscall(void,kbhit);