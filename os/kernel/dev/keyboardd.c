#include "drivers.h"

char vgetch_i(){//change to syscall
    // _vir2phy(char*,ch);
    return vgetchk();
}

int kbhit_i(){//change to syscall//kbhit ret 1
    // _vir2phy(int*,ifhit);
    return kbhitk();
}

_regist_syscall(void,vgetch);

_regist_syscall(void,kbhit);