#include "net.h"
#include "proc.h"

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
    printk("ip:%s,port:%d\n",sock->ip,sock->target_port);
    return connectk(sock);
}

int close_i(int sockfd){
    // 文件 fd 由文件系统释放 (fd 表项清空), 网络 fd 才走网卡
    fd_entry* e = fd_get_current(sockfd);
    if (e && e->type == FD_FILE) {
        e->type = FD_EMPTY;
        e->inode_id = 0;
        e->offset = 0;
        return 0;
    }
    return closek(sockfd);
}

_regist_syscall(void,close);
_regist_syscall(void,accept);
_regist_syscall(void,connect);
_regist_syscall(void,send);
_regist_syscall(void,recv);
