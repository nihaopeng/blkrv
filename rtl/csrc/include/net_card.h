#ifndef _NET_CARD_H_
#define _NET_CARD_H_

#include "vmem.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>


class net_card:public vmem
{
private:
    int pending_sockfd;         // accept/connect 进行中的 socket, -1 表示无
    int pending_connect_fd;     // connect 进行中的 socket, -1 表示无
    char* pending_send;         // 待发送的数据缓冲
    int pending_send_len;
    int pending_send_off;
    char* pending_recv;         // 接收缓冲
    int pending_recv_len;
    int pending_recv_off;
public:
    net_card(uint32_t size);
    ~net_card();
    int blk_accept_nb();
    int blk_connect_nb();
    int blk_send_nb();
    int blk_recv_nb();
    int blk_close();
    // 每 tick 检查命令寄存器, 非阻塞执行 DMA 传输
    int process(Bus* bus,uint32_t tick=0);
};

#endif // !_NET_CARD_H_
