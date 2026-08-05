#include "drivers.h"
#include<stdarg.h>
#include "str.h"
// int un_use=0;
int in_cache_frontp=0;
int in_cache_backp=0;
char in_cache[IO_CACHE];

int init_input(){
    in_cache_frontp=0;
    in_cache_backp=0;
    // memset_s(in_cache,0,IO_CACHE);
}

char vgetchk(){//change to syscall
    if(in_cache_frontp!=in_cache_backp){
        // printk("%d,%d\n",in_cache_frontp,in_cache_backp);
        char ch=in_cache[in_cache_frontp++];
        // printk("%d ",*ch);
        in_cache_frontp=mod(in_cache_frontp,IO_CACHE);
        return ch;
    }else{
        return 0;
    }
}

int kbhitk(){//change to syscall//kbhit ret 1
    if(in_cache_frontp!=in_cache_backp){
        return 1;
    }else{
        return 0;
    }
}


void keydown_interrupt_i(){
    uint32_t ch_int=*(uint32_t*)KEYBOARD_CACHE_ADDR;
    int next_bp;
    if((ch_int&0x000000ff)!=0){
        in_cache[in_cache_backp]=(char)(ch_int&0x000000ff);
        next_bp=mod(in_cache_backp+1,IO_CACHE);
        if(next_bp!=in_cache_frontp) in_cache_backp=next_bp; // 溢出保护: 满则丢弃
    }
    if((ch_int&0x0000ff00)!=0){
        in_cache[in_cache_backp]=(char)((ch_int&0x0000ff00)>>8);
        next_bp=mod(in_cache_backp+1,IO_CACHE);
        if(next_bp!=in_cache_frontp) in_cache_backp=next_bp;
    }
    if((ch_int&0x00ff0000)!=0){
        in_cache[in_cache_backp]=(char)((ch_int&0x00ff0000)>>16);
        next_bp=mod(in_cache_backp+1,IO_CACHE);
        if(next_bp!=in_cache_frontp) in_cache_backp=next_bp;
    }
}

_regist_syscall(void,keydown_interrupt);

int inputk(const char* fmt,...){
    int fmt_len=str_len(fmt);
    va_list args;
    va_start(args,fmt);
    for(int i=0;i<fmt_len;i++){
        if(fmt[i]=='%'){
            char fmts[FMT_STRING_SIZE];
            int p=0;
            int num=0;
            while(1){
                // print("getting ch\n");
                char ch=vgetchk();
                if(ch==10||ch==32){
                    printk("%c",ch);
                   if(p==0)continue;
                   else break;
                }else if(ch!=0){
                    printk("%c",ch);
                    if(ch==127){
                        p-=1;
                        fmts[p]=0;
                    }else{
                        fmts[p++]=ch;
                    }
                }
            }
            switch (fmt[i+1])
            {
                case 'c':
                    char* ch=va_arg(args,char*);
                    *ch=fmts[0];
                    break;
                case 's':
                    char* str=va_arg(args,char*);
                    str_cpy_s(fmts,str,0,--p);
                    break;
                case 'd':
                    int* va=va_arg(args,int*);
                    char tmp[12];
                    str_cpy_s(fmts,tmp,0,--p);
                    num=atoi(tmp);
                    *va=num;
                    break;
                default:
                    break;
            }
            i++;
        }
    }
    return 0;
}