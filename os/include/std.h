#include "set_gate.h"
#include "syscall.h"

void write_imp(void);

void set_syscall_gates(){
    _set_syscall_gate(4,&write_imp);
}

