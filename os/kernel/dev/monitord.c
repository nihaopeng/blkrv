#include "drivers.h"

int open_monitor_i(){
    open_monitor_k();
}

int close_monitor_i(){
    close_monitor_k();
}

_regist_syscall(void,open_monitor);

_regist_syscall(void,close_monitor);