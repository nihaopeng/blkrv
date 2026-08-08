#include "drivers.h"
#include "str.h"
#include "file.h"
#include <stdarg.h>

char out_cache_k[1024];

int init_out(){
    memset_s(out_cache_k,0,1024);
    return 0;
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
                    char ch=(char)va_arg(args,int);//所有参数均是4字节对齐；
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
    // 统一走帧缓存终端路径 (和 write(1) 相同), 由 screen 设备增量渲染
    tty_writek(out_cache_k, out_cache_n);
    // out_cache_mutex=0;
    return 0;
}
