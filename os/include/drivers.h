#ifndef _DRIVERS_H_
#define _DRIVERS_H_
#include "syscall.h"
#include "mm.h"

char out_cache[1024*1024];

_syscall2(int,vprint,char*,str,uint32_t,length);

int vprint_i(char* str,uint32_t length){
    for(int i=0;i<length;i++){
        char ch=*(out_cache+i);
        char* addr=(char*)SCREEN_CACHE1_ADDR+i;
        memset_i(addr,ch);
    }
    char* ctrl_addr=(char*)SCREEN_CTRL_ADDR+3;
    memset_i(ctrl_addr,1);
    return 0;
}


#endif // !_DRIVERS_H_
