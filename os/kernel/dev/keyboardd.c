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

int vgetch_i(char* ch){//change to syscall
    uint32_t p=0;
    __asm__ volatile(
        "csrr %0,0x181"
        :"=r"(p)
    );
    p=(p<<1)>>1;//去除mmu标志位
    ch=(char*)((void*)ch+p);
    // printk("%d,%d\n",in_cache_frontp,in_cache_backp);
    if(in_cache_frontp!=in_cache_backp){
        *ch=in_cache[in_cache_frontp++];
        in_cache_frontp=mod(in_cache_frontp,IO_CACHE);
    }else{
        *ch=0;
    }
}

int kbhit_i(int* ifhit){//change to syscall//kbhit ret 1
    uint32_t p=0;
    __asm__ volatile(
        "csrr %0,0x181"
        :"=r"(p)
    );
    p=(p<<1)>>1;//去除mmu标志位
    ifhit=(int*)((void*)ifhit+p);
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
    // printk("%d",ch);
    in_cache[in_cache_backp++]=ch;
    // in_cache_backp++;
    // printk("backp:%d\n",in_cache_backp);
    in_cache_backp=mod(in_cache_backp,IO_CACHE);
    // printk("test1");
}

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