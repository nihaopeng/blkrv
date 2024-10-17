#include "std.h"

int in_cache_mutex=0;
int in_cache_frontp=0,in_cache_backp=0;
int out_cache_mutex=0;
desc_table syscall_table;
desc_table inter_table;
char out_cache[IO_CACHE];
char in_cache[IO_CACHE];

_syscall2(int,vprint,char*,str,uint32_t,length);
_syscall0(char,vgetch);

int print(const char* fmt,...){//only support 'c' now;
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
                    out_cache[out_cache_n++]=(char)va;
                    break;
                case 's':
                    va_addr=va_n*4;
                    get_va(va_addr,va);
                    va_n++;
                    string_length=str_len((char*)va);
                    for(int s=0;s<string_length;s++){
                        out_cache[out_cache_n++]=*((char*)va+s);
                    }
                    break;
                case 'd':
                    va_addr=va_n*4;
                    get_va(va_addr,va);
                    va_n++;
                    itoa((int)va,s_t);
                    string_length=str_len(s_t);
                    for(int s=0;s<string_length;s++){
                        out_cache[out_cache_n++]=s_t[s];
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
    // out_cache_mutex=0;
    return 0;
}

int input(const char* fmt,...){
    int fmt_len=str_len(fmt);
    int va_n=1;
    uint32_t va=0;
    while(in_cache_mutex);
    in_cache_mutex=1;
    for(int i=0;i<fmt_len;i++){
        if(fmt[i]=='%'){
            char fmts[FMT_STRING_SIZE];
            int p=0;
            int va_addr=0;
            int num=0;
            while(1){
                char ch=vgetch();
                if(ch==10||ch==32){
                   if(p==0)continue;else break;
                }else{
                    fmts[p++]=ch;
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

// extern desc_table inter_table;

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

void regist_stdin(){//放在这里是为了避免vgetch_i被编译在GOT表中
    int* gdt_addr_vgetch=(int*)(&syscall_table[_NR_vgetch]);
    int* func_addr_vgetch=(int*)(&vgetch_i);
    _set_gate(gdt_addr_vgetch,func_addr_vgetch);
}


// extern desc_table syscall_table;

int vprint_i(char* str,uint32_t length){
    for(uint32_t i=0;i<length;i++){
        char ch=*(str+i);
        char* addr=(char*)SCREEN_CACHE1_ADDR+i;
        memset_i(addr,ch);
    }
    char* ctrl_addr=(char*)SCREEN_CTRL_ADDR+3;
    memset_i(ctrl_addr,1);
    return 0;
}

void regist_stdout(){//放在这里是为了避免vprint_i被编译在GOT表中
    int* gdt_addr_vprint=(int*)(&syscall_table[_NR_vprint]);
    int* func_addr_vprint=(int*)(&vprint_i);
    _set_gate(gdt_addr_vprint,func_addr_vprint);
}

void init_std(){
    regist_stdout();
    regist_stdin();
}