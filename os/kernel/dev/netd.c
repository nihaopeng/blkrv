#include "net.h"

int send_i(socket* sock,char* buf,uint32_t buf_length){
    _vir2phy(socket*,sock);
    _vir2phy(char*,buf);
    sendk(sock,buf,buf_length);
}

int recv_i(socket* sock,char* buf,uint32_t buf_length,int* status){
    _vir2phy(socket*,sock);
    _vir2phy(char*,buf);
    _vir2phy(int*,status);

    recvk(sock,buf,buf_length,status);
}

_regist_syscall(void,send);
_regist_syscall(void,recv);