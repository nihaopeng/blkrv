#include "drivers.h"

// int un_use=0;
int in_cache_frontp=0;
int in_cache_backp=0;
char in_cache[IO_CACHE];

int init_input(){
    in_cache_frontp=0;
    in_cache_backp=0;
    // memset_s(in_cache,0,IO_CACHE);
}

int vgetchk(char* ch){//change to syscall
    if(in_cache_frontp!=in_cache_backp){
        // printk("%d,%d\n",in_cache_frontp,in_cache_backp);
        *ch=in_cache[in_cache_frontp++];
        // printk("%d ",*ch);
        in_cache_frontp=mod(in_cache_frontp,IO_CACHE);
    }else{
        *ch=0;
    }
}

int kbhitk(int* ifhit){//change to syscall//kbhit ret 1
    if(in_cache_frontp!=in_cache_backp){
        *ifhit=1;
    }else{
        *ifhit=0;
    }
}


void keydown_interrupt_i(){
    uint32_t ch_int=*(uint32_t*)KEYBOARD_CACHE_ADDR;
    if((ch_int&0x000000ff)!=0){
        in_cache[in_cache_backp++]=(char)(ch_int&0x000000ff);
        in_cache_backp=mod(in_cache_backp,IO_CACHE);
    }
    if((ch_int&0x0000ff00)!=0){
        in_cache[in_cache_backp++]=(char)((ch_int&0x0000ff00)>>8);
        in_cache_backp=mod(in_cache_backp,IO_CACHE);
    }
    if((ch_int&0x00ff0000)!=0){
        in_cache[in_cache_backp++]=(char)((ch_int&0x00ff0000)>>16);
        in_cache_backp=mod(in_cache_backp,IO_CACHE);
    }
}

_regist_syscall(void,keydown_interrupt);

int inputk(const char* fmt,...){
    int fmt_len=str_len(fmt);
    int va_n=1;
    uint32_t va=0;
    for(int i=0;i<fmt_len;i++){
        if(fmt[i]=='%'){
            char fmts[FMT_STRING_SIZE];
            int p=0;
            int va_addr=0;
            int num=0;
            while(1){
                // print("getting ch\n");
                char ch=0;vgetch_i(&ch);
                if(ch==10||ch==32){
                    printk("%c",ch);
                   if(p==0)continue;
                   else break;
                }else if(ch>=32&&ch<=126){
                    printk("%c",ch);
                    fmts[p++]=ch;
                }else{
                    continue;
                }
            }
            switch (fmt[i+1])
            {
                case 'c':
                    va_addr=va_n*4;
                    get_va(va_addr,va);
                    va_n++;
                    *((char*)va)=fmts[0];
                    break;
                case 's':
                    va_addr=va_n*4;
                    get_va(va_addr,va);
                    va_n++;
                    str_cpy_s(fmts,(char*)va,0,--p);
                    break;
                case 'd':
                    va_addr=va_n*4;
                    get_va(va_addr,va);
                    va_n++;
                    char tmp[12];
                    str_cpy_s(fmts,tmp,0,--p);
                    num=atoi(tmp);
                    *((int*)va)=num;
                    break;
                default:
                    break;
            }
            i++;
        }
    }
    return 0;
}