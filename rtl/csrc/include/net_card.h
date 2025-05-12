#ifndef _NET_CARD_H_
#define _NET_CARD_H_

#include "vmem.h"
#include "ram.h"
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
// typedef struct dataframe
// {
//     /* data */
//     char ip[16];
//     uint32_t port;
//     uint32_t data_phy_addr;
//     uint32_t data_len;
// }dataframe;


class net_card:public vmem
{
private:
    /* data */
    ram* my_ram;
    pthread_t thread;
    int if_start_up;
public:
    net_card(uint32_t size);
    ~net_card();
    // dataframe* datafram_from_blkos();
    int blk_accept();
    int blk_connect();
    int blk_send();
    int blk_recv();
    int blk_close();
    static void* thread_function(void* arg);
    int process(rib* rib,uint32_t tick=0) override;
    int dma_link(ram* ram);
};

#endif // !_NET_CARD_H_
