#ifndef _NET_H_
#define _NET_H_
#include "ini.h"
#include "drivers.h"

typedef struct socket
{
    /* data */
    uint32_t origin_port;
    char ip[16];
    uint32_t target_port;
}socket;

int send(socket* sock,char* buf,uint32_t buf_length);

int sendk(socket* sock,char* buf,uint32_t buf_length);

int send_i(socket* sock,char* buf,uint32_t buf_length);

int recvk(socket* sock,char* buf,uint32_t buf_length,int* status);

int recv(socket* sock,char* buf,uint32_t buf_length,int* status);

int recv_i(socket* sock,char* buf,uint32_t buf_length,int* status);

void regist_send(int* dt_addr);

void regist_recv(int* dt_addr);

#endif // !_NET_H_
