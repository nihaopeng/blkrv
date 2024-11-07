#ifndef _NET_H_
#define _NET_H_

typedef struct socket
{
    /* data */
    uint32_t origin_port;
    char ip[16];
    uint32_t target_port;
}socket;

int send(socket sock,char* buf,uint32_t buf_length);

int recv(socket sock,char* buf,uint32_t buf_length,int* status);

#endif // !_NET_H_
