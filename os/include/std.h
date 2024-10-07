#ifndef _STD_H_
#define _STD_H_

#include "ini.h"
#include "console.h"
#include "set_gate.h"
#include "syscall.h"

int memset_i(char* addr,char ch){
    char *a = (addr);
    *(a) = (ch);
    return 0;
}

#define get_va(n,va) \
__asm__ volatile( \
    "add a1,s0,%1\n" \
    "lw %0,0(a1)\n"\
    :"=r"(va) \
    :"r"(n) \
);

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

int print(const char* fmt,...){//only support 'c' now;
    int fmt_len=str_len(fmt);
    int va_n=1;
    uint32_t va=0;
    uint32_t out_cache_n=0;
    for(int i=0;i<fmt_len;i++){
        if(fmt[i]=='%'){
            switch (fmt[i+1])
            {
                case 'c':
                    int va_addr=va_n*4;
                    get_va(va_addr,va);
                    va_n++;
                    out_cache[out_cache_n++]=(char)va;
                    break;
                case 's':
                    va_addr=va_n*4;
                    get_va(va_addr,va);
                    va_n++;
                    int string_length=str_len((char*)va);
                    for(int s=0;s<string_length;s++){
                        out_cache[out_cache_n++]=*((char*)va+s);
                    }
                    break;
                default:
                    break;
            }
            i++;
        }else{
            out_cache[out_cache_n++]=fmt[i];
        }
    }
    out_cache[out_cache_n]='\0';
    vprint(out_cache,out_cache_n);
}

void init_std(){
    int* gdt_addr=(int*)(&syscall_table[_NR_vprint]);
    int* func_addr=(int*)(&vprint_i);
    _set_gate(gdt_addr,func_addr);
}

#endif // !_STD_H_


