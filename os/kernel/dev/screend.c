#include "drivers.h"

int stdout=-1;
int stdout_start=0;
char out_cache_k[1024];

void set_stdout(int stdouts,int stdout_starts){
    //when you redirect to file, the stdout_start is start of file ptr
    stdout=stdouts;
    stdout_start=stdout_starts;
}

int printk(const char* fmt,...){//only support 'c' now;
    int fmt_len=str_len(fmt);
    // char out_cache[512];
    int va_n=1;
    uint32_t va=0;
    uint32_t out_cache_n=0;
    // while(out_cache_mutex);//给标准输出上锁
    // out_cache_mutex=1;
    for(int i=0;i<fmt_len;i++){
        if(fmt[i]=='%'){
            int va_addr=0;
            char s_t[33];
            int string_length=0;
            switch (fmt[i+1])
            {
                case 'c':
                    va_addr=va_n*4;
                    get_va(va_addr,va);
                    va_n++;
                    out_cache_k[out_cache_n++]=(char)va;
                    break;
                case 's':
                    va_addr=va_n*4;
                    get_va(va_addr,va);
                    va_n++;
                    string_length=str_len((char*)va);
                    for(int s=0;s<string_length;s++){
                        out_cache_k[out_cache_n++]=*((char*)va+s);
                    }
                    break;
                case 'd':
                    va_addr=va_n*4;
                    get_va(va_addr,va);
                    va_n++;
                    itoa((int)va,s_t);
                    string_length=str_len(s_t);
                    for(int s=0;s<string_length;s++){
                        out_cache_k[out_cache_n++]=s_t[s];
                    }
                    break;
                default:
                    break;
            }
            i++;
        }else{
            out_cache_k[out_cache_n++]=fmt[i];
        }
    }
    out_cache_k[out_cache_n]='\0';
    // vprint_i(out_cache_k,out_cache_n);//不使用这个函数是因为里面有satp寄存器读取
    for(uint32_t i=0;i<out_cache_n;i++){
        char ch=*(out_cache_k+i);
        char* addr=(char*)SCREEN_CACHE1_ADDR+i;
        *addr=ch;
    }
    char* ctrl_addr=(char*)SCREEN_CTRL_ADDR+3;
    *ctrl_addr=1;
    // out_cache_mutex=0;
    return 0;
}

int vprint_i(char* str,uint32_t length){
    // str=(char*)user_to_global((void*)str);
    // printk("%d\n",str);
    if(stdout==-1){
        uint32_t p=0;
        __asm__ volatile(
            "csrrw %0,0x181,zero"
            :"=r"(p)
        );
        p=(p<<1)>>1;//去除mmu标志位
        str=(char*)((void*)str+p);
        // printk("%d\n",str);
        for(uint32_t i=0;i<length;i++){
            char ch=*(str+i);
            char* addr=(char*)SCREEN_CACHE1_ADDR+i;
            *addr=ch;
        }
        char* ctrl_addr=(char*)SCREEN_CTRL_ADDR+3;
        *ctrl_addr=1;
    }else{
        write_i(stdout,str,stdout_start,length);
    }
    return 0;
}

void regist_vprint(int* gdt_addr_vprint){//放在这里是为了避免vprint_i被编译在GOT表中
    // 
    int* func_addr_vprint=(int*)(&vprint_i);
    _set_gate(gdt_addr_vprint,func_addr_vprint);
    // _set_syscall_gate(_NR_vprint,func_addr_vprint);
}