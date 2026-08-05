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

int send(int sockfd,char* buf,uint32_t buf_length);

int sendk(int sockfd,char* buf,uint32_t buf_length);

int send_i(int sockfd,char* buf,uint32_t buf_length);

int recvk(int sockfd,char* buf,uint32_t buf_length);

int recv(int sockfd,char* buf,uint32_t buf_length);

int recv_i(int sockfd,char* buf,uint32_t buf_length);

int accept(socket* sock);

int acceptk(socket* sock);

int accept_i(socket* sock);

int connect(socket* sock);

int connectk(socket* sock);

int connect_i(socket* sock);

int close(int sockfd);

int closek(int sockfd);

int close_i(int sockfd);

void regist_accept(int* dt_addr);

void regist_connect(int* dt_addr);

void regist_close(int* dt_addr);

void regist_send(int* dt_addr);

void regist_recv(int* dt_addr);

void regist_nic_interrupt(int* dt_addr);

extern volatile int nic_done;

#endif // !_NET_H_
