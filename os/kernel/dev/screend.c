#include "drivers.h"

char out_cache[IO_CACHE];
int out_cache_mutex=0;
extern desc_table syscall_table;

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

void regist_stdout(){//放在这里是为了避免vprint_i被编译在GOT表中
    int* gdt_addr_vprint=(int*)(&syscall_table[_NR_vprint]);
    int* func_addr_vprint=(int*)(&vprint_i);
    _set_gate(gdt_addr_vprint,func_addr_vprint);
}