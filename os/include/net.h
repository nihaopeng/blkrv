#ifndef _NET_H_
#define _NET_H_

typedef struct socket
{
    /* data */
    uint8_t origin_port;
    uint8_t ip[4];
    uint8_t target_port;
    uint16_t reserved;
}socket;

int send(socket sock,char* buf,uint32_t buf_length);

int recv(socket sock,char* buf,uint32_t buf_length);

#endif // !_NET_H_
