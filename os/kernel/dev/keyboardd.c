#include "drivers.h"

int in_cache_frontp=0,in_cache_backp=0;
char in_cache[IO_CACHE];

int vgetch_i(){
    if(in_cache_frontp!=in_cache_backp){
        char ch=in_cache[in_cache_frontp];
        in_cache_frontp++;
        in_cache_frontp=mod(in_cache_frontp,IO_CACHE);
        return ch;
    }else{
        return -1;
    }
}

void keydown_interrupt(){
    char ch;
    __asm__ volatile(
        "li a0,%1\n"
        "lb %0,0(a0)\n"
        :"=r"(ch)
        :"i"(KEYBOARD_CACHE_ADDR)
    );
    in_cache[in_cache_backp++]=ch;
}

void regist_stdin(int* dt_addr){//放在这里是为了避免vgetch_i被编译在GOT表中
    int* func_addr_vgetch=(int*)(&vgetch_i);
    // _set_syscall_gate(_NR_vgetch,func_addr_vgetch);
    _set_gate(dt_addr,func_addr_vgetch);
}

void regist_keydown_int(int* dt_addr){
    int* func_addr_keydown_interrupt=(int*)(&keydown_interrupt);
    _set_gate(dt_addr,func_addr_keydown_interrupt);
}