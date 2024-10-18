#ifndef _SET_GATE_H_
#define _SET_GATE_H_

#define _set_gate(dt_addr,program_addr) \
__asm__ volatile( \
    "add a1,zero,%0\n" \
    "add a2,zero,%1\n" \
    "sw a2,0(a1)\n" \
    : \
    :"r"(dt_addr),"r"(program_addr) \
);
// #define _set_int_gate(int_table_n_addr,program_addr) \
// _set_gate(int_table_n_addr,program_addr)

// #define _set_syscall_gate(syscall_table_n_addr,program_addr) \
// _set_gate(syscall_table_n_addr,program_addr)


#endif // !_SET_GATE_H_


