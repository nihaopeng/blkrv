#include "std.h"
#include<stdarg.h>

int out_cache_mutex=0;
int in_cache_mutex=0;
char out_cache[IO_CACHE];

_syscall0(int,exit);
_syscall4(int,exec,uint32_t,inode_id,int,stdout,char**,para,uint32_t,para_num);
_syscall0(char,vgetch);
_syscall0(int,kbhit);
_syscall0(int,powoff);
_syscall2(int,vprint,char*,str,uint32_t,length);
_syscall4(int,read,uint32_t,inode_id,char*,buf,uint32_t,start,uint32_t,count);
_syscall4(int,write,uint32_t,inode_id,char*,buf,uint32_t,start,uint32_t,length);
_syscall3(int,create,char*,file_path,char,type,uint32_t*,inode_id);
_syscall1(int,open,const char*,file_path);
_syscall1(int,free,void*,pointer);
_syscall1(void*,malloc,uint32_t,size);
_syscall3(int,send,int,sockfd,char*,buf,uint32_t,buf_length);
_syscall3(int,recv,int,sockfd,char*,buf,uint32_t,buf_length);
_syscall4(int,draw_label,point*,p1,char*,str,color*,c,int,font);
_syscall4(int,draw_triangle,point*,p1,point*,p2,point*,p3,color*,c);
_syscall0(int,flush);
_syscall0(int,open_monitor);
_syscall0(int,close_monitor);
_syscall2(int,finfo,uint32_t,inode_id,inode*,finode);
_syscall1(int,delete,uint32_t,inode_id);
_syscall4(int,draw_jpg,uint32_t,inode_id,uint32_t,size,uint32_t,x,uint32_t,y);
_syscall4(int,draw_png,uint32_t,inode_id,uint32_t,size,uint32_t,x,uint32_t,y);
_syscall1(int,accept,socket*,sock);
_syscall1(int,connect,socket*,sock);
_syscall1(int,close,int,sockfd);

int print(const char* fmt,...){//only support 'c' now;
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
                    char ch=(char)va_arg(args,int);
                    out_cache[out_cache_n++]=ch;
                    break;
                case 's':
                    char* str=va_arg(args,char*);
                    string_length=str_len(str);
                    for(int s=0;s<string_length;s++){
                        out_cache[out_cache_n++]=*((char*)str+s);
                    }
                    break;
                case 'd':
                    itoa(va_arg(args,int),s_t);
                    string_length=str_len(s_t);
                    for(int s=0;s<string_length;s++){
                        out_cache[out_cache_n++]=s_t[s];
                    }
                    break;
                case 'x':
                    xtoa(va_arg(args,int),s_t);
                    string_length=str_len(s_t);
                    for(int s=0;s<string_length;s++){
                        out_cache[out_cache_n++]=s_t[s];
                    }
                    break;
                case 'f':
                    int int_f=va_arg(args,int);
                    float *fp = (float*)&int_f;
                    float f = *fp;
                    ftoa(f,s_t,6);
                    string_length=str_len(s_t);
                    for(int s=0;s<string_length;s++){out_cache[out_cache_n++]=s_t[s];}
                    break;
                default:
                    break;
            }
            i++;
        }else{
            out_cache[out_cache_n++]=fmt[i];
        }
    }
    va_end(args);
    out_cache[out_cache_n]='\0';
    vprint(out_cache,out_cache_n);
    // out_cache_mutex=0;
    return 0;
}

int input(const char* fmt,...){
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
                char ch=vgetch();
                if(ch==10||ch==32){
                    // printk("%c",ch);
                   if(p==0)continue;
                   else break;
                }else if(ch!=0){
                    // printk("%c",ch);
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

int getline(char* str){
    uint32_t p=0;
    while(1){
        // print("getting ch\n");
        char ch=vgetch();
        if(ch==10){
            // printk("enter]%c",ch);
            break;
        }else if(ch!=0){
            // printk("%d ",ch);
            if(ch==127){
                p-=1;
                str[p]=0;
            }else{
               str[p++]=ch;
            }
        }
    }
    return p;
}

int shutdown(){
    print("\n---!powoff now!---\n");
    powoff();
}