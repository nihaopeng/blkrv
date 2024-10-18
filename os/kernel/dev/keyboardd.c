#include "drivers.h"

int in_cache_frontp=0,in_cache_backp=0;
char in_cache[IO_CACHE];

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

void regist_stdin(dt_addr){//放在这里是为了避免vgetch_i被编译在GOT表中
    int* func_addr_vgetch=(int*)(&vgetch_i);
    // _set_syscall_gate(_NR_vgetch,func_addr_vgetch);
    _set_gate(dt_addr,func_addr_vgetch);
}