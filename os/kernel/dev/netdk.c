#include "net.h"
#include "str.h"

volatile int nic_done = 0;

void nic_interrupt_i(){
    nic_done = 1;
}

_regist_syscall(void, nic_interrupt);

int init_net(){
    *(uint32_t*)NIC_CTRL_ADDR = 0;
}

int recvk(int sockfd, char* buf, uint32_t buf_length){
    if (*(uint32_t*)NIC_CTRL_ADDR == 0) {
        *(uint32_t*)(NIC_CTRL_ADDR + 4)  = sockfd;
        *(uint32_t*)(NIC_CTRL_ADDR + 8)  = (uint32_t)buf;
        *(uint32_t*)(NIC_CTRL_ADDR + 12) = buf_length;
        nic_done = 0;
        *(uint32_t*)NIC_CTRL_ADDR = 3;
        int nic_timeout = 0;
        while (!nic_done && ++nic_timeout < 50000000); // 超时保护, 约 5 秒
        return *(uint32_t*)(NIC_CTRL_ADDR + 16);
    }
    return -1;
}

int sendk(int sockfd, char* buf, uint32_t buf_length){
    if (*(uint32_t*)NIC_CTRL_ADDR == 0) {
        *(uint32_t*)(NIC_CTRL_ADDR + 4)  = sockfd;
        *(uint32_t*)(NIC_CTRL_ADDR + 8)  = (uint32_t)buf;
        *(uint32_t*)(NIC_CTRL_ADDR + 12) = buf_length;
        nic_done = 0;
        *(uint32_t*)NIC_CTRL_ADDR = 2;
        while (!nic_done);
        return 0;
    }
    return -1;
}

int acceptk(socket* sock){
    if (*(uint32_t*)NIC_CTRL_ADDR == 0) {
        str_cpy_s(sock->ip, (char*)(NIC_CTRL_ADDR + 4), 0, 16);
        *(uint32_t*)(NIC_CTRL_ADDR + 20) = sock->target_port;
        nic_done = 0;
        *(uint32_t*)NIC_CTRL_ADDR = 1;
        while (!nic_done);
        return *(uint32_t*)(NIC_CTRL_ADDR + 28);
    }
    return -1;
}

int connectk(socket* sock){
    if (*(uint32_t*)NIC_CTRL_ADDR == 0) {
        str_cpy_s(sock->ip, (char*)(NIC_CTRL_ADDR + 4), 0, 16);
        *(uint32_t*)(NIC_CTRL_ADDR + 20) = sock->target_port;
        nic_done = 0;
        *(uint32_t*)NIC_CTRL_ADDR = 4;
        while (!nic_done);
        return *(uint32_t*)(NIC_CTRL_ADDR + 28);
    }
    return -1;
}

int closek(int sockfd){
    if (*(uint32_t*)NIC_CTRL_ADDR == 0) {
        *(uint32_t*)(NIC_CTRL_ADDR + 4) = sockfd;
        nic_done = 0;
        *(uint32_t*)NIC_CTRL_ADDR = 5;
        while (!nic_done);
        return 0;
    }
    return -1;
}
