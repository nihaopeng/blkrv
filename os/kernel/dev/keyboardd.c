#include "drivers.h"

char in_cache[IO_CACHE];
int in_cache_mutex=0;
int in_cache_frontp=0,in_cache_backp=0;
extern desc_table inter_table;

int vgetch_i(){
    if(in_cache_frontp!=in_cache_backp){
        char ch=in_cache[in_cache_frontp];
        in_cache_frontp++;
        in_cache_frontp=mod(in_cache_frontp,1024*1024);
        return ch;
    }else{
        return -1;
    }
}

void regist_stdin(){//放在这里是为了避免vgetch_i被编译在GOT表中
    int* gdt_addr_vgetch=(int*)(&syscall_table[_NR_vgetch]);
    int* func_addr_vgetch=(int*)(&vgetch_i);
    _set_gate(gdt_addr_vgetch,func_addr_vgetch);
}