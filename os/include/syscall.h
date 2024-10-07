#ifndef _SYSCALL_H_
#define _SYSCALL_H_

#include "ini.h"

#define _syscall0(type,name) \
type name(void) \
{ \
    uint32_t __res;\
    __asm__ volatile(\
        "li a7, %1\n"\
        "ecall\n"\
        "mv %0, a0\n"\
        : "=r"(__res)\
        : "i"(_NR_##name)\
        : "memory");\
    if (__res>=0) \
	    return (type)__res; \
    return -1; \
}

#define _syscall1(type,name,atype,a) \
type name(atype a) \
{ \
    uint32_t __res;\
    __asm__ volatile (\
        "li a7, %1\n"\
        "mv a0, %2\n"\
        "ecall\n"\
        "mv %0, a0\n"\
        : "=r"(__res)\
        : "i"(_NR_##name),"r"((uint32_t)a)\
        : "memory");\
    if (__res>=0) \
	    return (type) __res; \
    return -1; \
}

#define _syscall2(type,name,atype,a,btype,b) \
type name(atype a,btype b) \
{ \
    uint32_t __res;\
    __asm__ volatile (\
        "li a7, %1\n"\
        "mv a0, %2\n"\
        "mv a1, %3\n"\
        "ecall\n"\
        "mv %0, a0\n"\
        : "=r"(__res)\
        : "i"(_NR_##name), "r"((uint32_t)a), "r"((uint32_t)b)\
        : "memory");\
    if (__res>=0) \
	    return (type) __res; \
    return -1; \
}


#define _syscall3(type,name,atype,a,btype,b,ctype,c) \
type name(atype a,btype b,ctype c) \
{ \
    uint32_t __res; \
    __asm__ volatile (\
        "li a7, %1\n" \
        "mv a0, %2\n"\
        "mv a1, %3\n"\
        "mv a2, %4\n" \
        "ecall\n" \
        "mv %0, a0\n" \
        : "=r"(__res) \
        : "i"(_NR_##name), "r"((uint32_t)a), "r"((uint32_t)b) , "r"((uint32_t)c) \
        : "memory"); \
    if (__res>=0) \
	    return (type) __res; \
    return -1; \
}

#endif // !_SYSCALL_H_



