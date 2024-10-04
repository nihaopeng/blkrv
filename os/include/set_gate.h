#include "ini.h"

#define int_base_addr 0
#define syscall_base_addr 1024

#define _set_gate(base_addr,call_num,program_addr) \
__asm__ volatile( \
    "addi a1,x0,0\n" \
    "addi a1,a1,%0\n" \
    "addi a1,a1,%1\n" \
    "la a2,%2\n" \
    "sw a2,0(a1)\n" \
    : \
    :"i"((uint32_t)base_addr),"i"((uint32_t)(call_num<<2)),"i"((uint32_t)program_addr) \
    : "memory", "a1", "a2" \
);

#define _set_int_gate(call_num,program_addr) \
_set_gate(int_base_addr,call_num,program_addr)

#define _set_syscall_gate(call_num,program_addr) \
_set_gate(syscall_base_addr,call_num,program_addr)
