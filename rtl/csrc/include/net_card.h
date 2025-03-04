#ifndef _NET_CARD_H_
#define _NET_CARD_H_

#include "vmem.h"
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
typedef struct sok
{
    /* data */
    char ip[16];
    uint32_t port;
    char* data;
    uint32_t data_len;
}sok;


class net_card:public vmem
{
private:
    /* data */
    std::string server_ip;
    int server_port;
    int sockfd;
    pthread_t thread;
    int if_start_up;
public:
    net_card(uint32_t size);
    ~net_card();
    sok* sok_from_blkos();
    void connect2server(char ip[16],uint32_t port);
    void send_message(const char* message,const uint32_t data_len);
    std::string receive_message();
    static void* thread_function(void* arg);
    void process(Vtop* top,uint32_t tick=0) override;
};

#endif // !_NET_CARD_H_
