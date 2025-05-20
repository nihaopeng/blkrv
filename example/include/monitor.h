#include "ini.h"
#include "set_gate.h"
#include "syscall.h"
#include "std.h"

int open_monitor();

int open_monitor_i();

int open_monitor_k();

int close_monitor();

int close_monitor_i();

int close_monitor_k();

void regist_open_monitor(int* dt_addr);

void regist_close_monitor(int* dt_addr);