#include "drivers.h"
#include <stdarg.h>

int stdout=-1;
int stdout_start=0;
char out_cache_k[1024];

int init_out(){
    stdout=-1;
    stdout_start=0;
    memset_s(out_cache_k,0,1024);
}

void set_stdout(int stdouts,int stdout_starts){
    //when you redirect to file, the stdout_start is start of file ptr
    stdout=stdouts;
    stdout_start=stdout_starts;
}

int printk(const char* fmt,...){//放在这里为了访问out_cache
    int fmt_len=str_len(fmt);
    // char out_cache[512];
    va_list args;
    va_start(args,fmt);
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
                    char ch= va_arg(args,char);
                    out_cache_k[out_cache_n++]=ch;
                    break;
                case 's':
                    char* str=va_arg(args,char*);
                    string_length=str_len(str);
                    for(int s=0;s<string_length;s++){
                        out_cache_k[out_cache_n++]=*((char*)str+s);
                    }
                    break;
                case 'd':
                    itoa(va_arg(args,int),s_t);
                    string_length=str_len(s_t);
                    for(int s=0;s<string_length;s++){
                        out_cache_k[out_cache_n++]=s_t[s];
                    }
                    break;
                case 'x':
                    xtoa(va_arg(args,int),s_t);
                    string_length=str_len(s_t);
                    for(int s=0;s<string_length;s++){
                        out_cache_k[out_cache_n++]=s_t[s];
                    }
                    break;
                // case 'f':
                //     int int_f=va_arg(args,int);
                //     float *fp = (float*)&int_f;
                //     float f = *fp;
                //     ftoa(f,s_t,6);
                //     string_length=str_len(s_t);
                //     for(int s=0;s<string_length;s++){out_cache_k[out_cache_n++]=s_t[s];}
                //     break;
                default:
                    break;
            }
            i++;
        }else{
            out_cache_k[out_cache_n++]=fmt[i];
        }
    }
    va_end(args);
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
        _vir2phyk(char*,str);
        // printk("%d\n",str);
        for(uint32_t i=0;i<length;i++){
            char ch=*(str+i);
            char* addr=(char*)SCREEN_CACHE1_ADDR+i;
            *addr=ch;
        }
        char* ctrl_addr=(char*)SCREEN_CTRL_ADDR+3;
        *ctrl_addr=1;
    }else{
        writek(stdout,str,stdout_start,length);
    }
    return 0;
}

_regist_syscall(void,vprint);//放在这里是为了避免vprint_i被编译在GOT表中