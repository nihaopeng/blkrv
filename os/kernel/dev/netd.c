#include "net.h"

int send_i(socket* sock,char* buf,uint32_t buf_length){
    _vir2phyk(socket*,sock);
    _vir2phyk(char*,buf);
    
    printk("sock:%x\n",sock);
    sendk(sock,buf,buf_length);
}

int recv_i(socket* sock,char* buf,uint32_t buf_length){
    _vir2phyk(socket*,sock);
    _vir2phyk(char*,buf);
    // _vir2phyk(int*,status);
    return recvk(sock,buf,buf_length);
}

_regist_syscall(void,send);
_regist_syscall(void,recv);