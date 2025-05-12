#include "net.h"

int send_i(int sockfd,char* buf,uint32_t buf_length){
    _vir2phyk(char*,buf);
    // printk("sock:%x\n",sock);
    sendk(sockfd,buf,buf_length);
}

int recv_i(int sockfd,char* buf,uint32_t buf_length){
    _vir2phyk(char*,buf);
    // _vir2phyk(int*,status);
    return recvk(sockfd,buf,buf_length);
}

int accept_i(socket* sock){
    _vir2phyk(socket*,sock);
    return acceptk(sock);
}

int connect_i(socket* sock){
    _vir2phyk(socket*,sock);
    return connectk(sock);
}

int close_i(int sockfd){
    return closek(sockfd);
}

_regist_syscall(void,close);
_regist_syscall(void,accept);
_regist_syscall(void,connect);
_regist_syscall(void,send);
_regist_syscall(void,recv);