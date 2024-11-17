#include "drivers.h"

int in_cache_frontp=0,in_cache_backp=0;
char in_cache[IO_CACHE];

int vgetch_i(char* ch){//change to syscall
    if(in_cache_frontp!=in_cache_backp){
        *ch=in_cache[in_cache_frontp];
        in_cache_frontp++;
        in_cache_frontp=mod(in_cache_frontp,IO_CACHE);
    }
}

int kbhit_i(int* ifhit){//change to syscall
    if(in_cache_frontp!=in_cache_backp){
        *ifhit=1;
    }else{
        *ifhit=0;
    }
}

void keydown_interrupt(){
    char ch;
    __asm__ volatile(
        "li a0,%1\n"
        "lbu %0,0(a0)\n"
        :"=r"(ch)
        :"i"(KEYBOARD_CACHE_ADDR)
    );
    in_cache[in_cache_backp]=ch;
    in_cache_backp++;
    in_cache_backp=mod(in_cache_backp,IO_CACHE);
}

void regist_keydown_int(int* dt_addr){
    int* func_addr_keydown_interrupt=(int*)(&keydown_interrupt);
    _set_gate(dt_addr,func_addr_keydown_interrupt);
}

void regist_vgetch(int* dt_addr){
    int* func_addr_vgetch_i=(int*)(&vgetch_i);
    _set_gate(dt_addr,func_addr_vgetch_i);
}

void regist_kbhit(int* dt_addr){
    int* func_addr_kbhit_i=(int*)(&kbhit_i);
    _set_gate(dt_addr,func_addr_kbhit_i);
}