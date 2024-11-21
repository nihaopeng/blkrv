#include "net.h"

int send_i(socket* sock,char* buf,uint32_t buf_length){
    uint32_t p=0;
    __asm__ volatile(
        "csrr %0,0x181"
        :"=r"(p)
    );
    p=(p<<1)>>1;//去除mmu标志位
    sock=(socket*)((void*)sock+p);
    buf=(char*)((void*)buf+p);

    sendk(sock,buf,buf_length);
}

int recv_i(socket* sock,char* buf,uint32_t buf_length,int* status){
    uint32_t p=0;
    __asm__ volatile(
        "csrr %0,0x181"
        :"=r"(p)
    );
    p=(p<<1)>>1;//去除mmu标志位
    sock=(socket*)((void*)sock+p);
    buf=(char*)((void*)buf+p);
    status=(int*)((void*)status+p);

    recvk(sock,buf,buf_length,status);
}

void regist_send(int* dt_addr){
    int* func_addr_send_i=(int*)(&send_i);
    _set_gate(dt_addr,func_addr_send_i);
}

void regist_recv(int* dt_addr){
    int* func_addr_recv_i=(int*)(&recv_i);
    _set_gate(dt_addr,func_addr_recv_i);
}