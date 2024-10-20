#include "std.h"

_syscall2(int,vprint,char*,str,uint32_t,length);
_syscall0(int,powoff);

int out_cache_mutex=0;
int in_cache_mutex=0;
char out_cache[IO_CACHE];

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
    for(int i=0;i<fmt_len;i++){
        if(fmt[i]=='%'){
            char fmts[FMT_STRING_SIZE];
            int p=0;
            int va_addr=0;
            int num=0;
            while(1){
                char ch=vgetch();
                if(ch==10||ch==32){
                    print("%c",ch);
                   if(p==0)continue;
                   else break;
                }else if(ch>=32&&ch<=126){
                    print("%c",ch);
                    fmts[p++]=ch;
                }else{
                    continue;
                }
            }
            // int l=str_len(fmts);
            // for(int s=0;s<l;s++){
            //     print("%d\n",fmts[s]);
            // }
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

int shutdown(){
    print("\n---!powoff now!---\n");
    powoff();
}