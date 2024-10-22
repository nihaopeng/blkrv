#ifndef _SYSCALL_H_
#define _SYSCALL_H_

#include "ini.h"

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

#define _syscall4(type,name,atype,a,btype,b,ctype,c,dtype,d) \
type name(atype a,btype b,ctype c,dtype d) \
{ \
    type __res; \
    __asm__ volatile (\
        "li a7, %1\n"\
        "mv a0, %2\n"\
        "mv a1, %3\n"\
        "mv a2, %4\n"\
        "mv a3, %5\n"\
        "ecall\n" \
        "mv %0, a0\n" \
        : "=r"(__res) \
        : "i"(_NR_##name), "r"(a), "r"(b) , "r"(c), "r"(d) \
        : "memory"); \
    return (type) __res; \
}

#endif // !_SYSCALL_H_



