#ifndef _SYSCALL_H_
#define _SYSCALL_H_

#include "ini.h"

//将传入的地址转换为对应虚拟地址下的物理地址
//0x181是satp的副本，当前处于mmu不使能状态
#define _vir2phy(type,addr,satp) do{\
    uint32_t addrt=(uint32_t)addr;\
    uint32_t pte1=*(uint32_t*)((satp&0xfffff000)+((addrt>>22)<<2));\
    uint32_t pte2=*(uint32_t*)((pte1&0xfffff000)+(((addrt&0x003ff000)>>12)<<2));\
    addr=(type)((pte2&0xfffff000)+(addrt&0x00000fff));\
}while(0)

#define _vir2phyk(type,addr) do{\
    uint32_t p=0;\
    __asm__ volatile("csrr %0,0x181":"=r"(p));\
    _vir2phy(type,addr,p);\
}while(0)


#define _regist_syscall(type,name) \
type regist_##name(int* dt_addr) \
{ \
    int* func_addr_##name=(int*)(&name##_i);\
    _set_gate(dt_addr,func_addr_##name);\
}

#define _syscall0(type,name) \
type name(void) \
{ \
    type __res;\
    __asm__ volatile(\
        "li a7, %1\n"\
        "ecall\n"\
        "mv %0, a0\n"\
        : "=r"(__res)\
        : "i"(_NR_##name)\
        : "memory");\
    return (type) __res; \
}

#define _syscall1(type,name,atype,a) \
type name(atype a) \
{ \
    type __res;\
    __asm__ volatile (\
        "li a7, %1\n"\
        "mv a0, %2\n"\
        "ecall\n"\
        "mv %0, a0\n"\
        : "=r"(__res)\
        : "i"(_NR_##name),"r"(a)\
        : "memory");\
	return (type) __res; \
}

#define _syscall2(type,name,atype,a,btype,b) \
type name(atype a,btype b) \
{ \
    type __res;\
    __asm__ volatile (\
        "li a7, %1\n"\
        "mv a0, %2\n"\
        "mv a1, %3\n"\
        "ecall\n"\
        "mv %0, a0\n"\
        : "=r"(__res)\
        : "i"(_NR_##name), "r"(a), "r"(b)\
        : "memory");\
    return (type) __res; \
}


#define _syscall3(type,name,atype,a,btype,b,ctype,c) \
type name(atype a,btype b,ctype c) \
{ \
    type __res; \
    __asm__ volatile (\
        "li a7, %1\n" \
        "mv a0, %2\n"\
        "mv a1, %3\n"\
        "mv a2, %4\n" \
        "ecall\n" \
        "mv %0, a0\n" \
        : "=r"(__res) \
        : "i"(_NR_##name), "r"(a), "r"(b) , "r"(c) \
        : "memory"); \
    return (type) __res; \
}

//to avoid compile error
#define _syscall4(type,name,atype,a,btype,b,ctype,c,dtype,d) \
type name(atype a,btype b,ctype c,dtype d) \
{ \
    type __res; \
    __asm__ volatile (\
        "li a7, %1\n"\
        "mv t0, %2\n"\
        "mv t1, %3\n"\
        "mv t2, %4\n"\
        "mv t3, %5\n"\
        "mv a0, t0\n"\
        "mv a1, t1\n"\
        "mv a2, t2\n"\
        "mv a3, t3\n"\
        "ecall\n" \
        "mv %0, a0\n" \
        : "=r"(__res) \
        : "i"(_NR_##name), "r"(a), "r"(b) , "r"(c), "r"(d) \
        : "memory"); \
    return (type) __res; \
}

#endif // !_SYSCALL_H_



