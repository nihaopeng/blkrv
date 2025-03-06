#include "drivers.h"

int open_monitor_i(){
    open_monitor_k();
}

int close_monitor_i(){
    close_monitor_k();
}

void regist_open_monitor(int* dt_addr){
    int* func_addr_open_monitor_i=(int*)(&open_monitor_i);
    _set_gate(dt_addr,func_addr_open_monitor_i);
}

void regist_close_monitor(int* dt_addr){
    int* func_addr_close_monitor_i=(int*)(&close_monitor_i);
    _set_gate(dt_addr,func_addr_close_monitor_i);
}