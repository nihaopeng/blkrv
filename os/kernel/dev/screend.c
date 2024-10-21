#include "drivers.h"

_syscall2(int,vprint,char*,str,uint32_t,length);

int vprint_i(char* str,uint32_t length){
    for(uint32_t i=0;i<length;i++){
        char ch=*(str+i);
        char* addr=(char*)SCREEN_CACHE1_ADDR+i;
        memset_i(addr,ch);
    }
    char* ctrl_addr=(char*)SCREEN_CTRL_ADDR+3;
    memset_i(ctrl_addr,1);
    return 0;
}

void regist_vprint(int* gdt_addr_vprint){//放在这里是为了避免vprint_i被编译在GOT表中
    // 
    int* func_addr_vprint=(int*)(&vprint_i);
    _set_gate(gdt_addr_vprint,func_addr_vprint);
    // _set_syscall_gate(_NR_vprint,func_addr_vprint);
}