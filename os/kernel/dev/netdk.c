#include "net.h"
#include "str.h"
uint32_t rData_flag=0;

int init_net(){
    *(uint32_t*)NIC_CTRL_ADDR==0;
}

int recvk(int sockfd,char* buf,uint32_t buf_length){
    if(*(uint32_t*)NIC_CTRL_ADDR==0){
        *(uint32_t*)(NIC_CTRL_ADDR+4)=sockfd;
        *(uint32_t*)(NIC_CTRL_ADDR+8)=(uint32_t)buf-RAM_START;
        *(uint32_t*)(NIC_CTRL_ADDR+12)=buf_length;
        *(uint32_t*)NIC_CTRL_ADDR=3;
        while(*(uint32_t*)NIC_CTRL_ADDR==3);//网卡通知接收结束，阻塞
        return *(uint32_t*)(NIC_CTRL_ADDR+16);//返回接收数据长度
    }
}

int sendk(int sockfd,char* buf,uint32_t buf_length){
    if(*(uint32_t*)NIC_CTRL_ADDR==0){
        *(uint32_t*)(NIC_CTRL_ADDR+4)=sockfd;
        // printk("sendk:buf:%x,%s\n",buf,buf);
        *(uint32_t*)(NIC_CTRL_ADDR+8)=(uint32_t)buf-RAM_START;
        *(uint32_t*)(NIC_CTRL_ADDR+12)=buf_length;
        *(uint32_t*)NIC_CTRL_ADDR=2;
        while(*(uint32_t*)NIC_CTRL_ADDR==2);//网卡通知发送结束，阻塞
        return 0;
    }
}

int acceptk(socket* sock){
    if(*(uint32_t*)NIC_CTRL_ADDR==0){
        str_cpy_s(sock->ip,(char*)(NIC_CTRL_ADDR+4),0,16);
        *(uint32_t*)(NIC_CTRL_ADDR+20)=sock->target_port;
        *(uint32_t*)NIC_CTRL_ADDR=1;
        while(*(uint32_t*)NIC_CTRL_ADDR==1);//网卡通知连接成功，阻塞
        return *(uint32_t*)(NIC_CTRL_ADDR+28);//返回socket fd
    }
}

int connectk(socket* sock){
    if(*(uint32_t*)NIC_CTRL_ADDR==0){
        str_cpy_s(sock->ip,(char*)(NIC_CTRL_ADDR+4),0,16);
        printk("ip:%s,port:%d\n",(char*)(NIC_CTRL_ADDR+4),sock->target_port);
        *(uint32_t*)(NIC_CTRL_ADDR+20)=sock->target_port;
        *(uint32_t*)NIC_CTRL_ADDR=4;
        // printk("ctrl:%d\n",*(uint32_t*)NIC_CTRL_ADDR);
        while(*(uint32_t*)NIC_CTRL_ADDR==4);//网卡通知连接成功，阻塞
        return *(uint32_t*)(NIC_CTRL_ADDR+28);//返回socket fd
    }
}

int closek(int sockfd){
    if(*(uint32_t*)NIC_CTRL_ADDR==0){
        *(uint32_t*)(NIC_CTRL_ADDR+4)=sockfd;
        *(uint32_t*)NIC_CTRL_ADDR=5;
        while(*(uint32_t*)NIC_CTRL_ADDR==5);//网卡通知连接成功，阻塞
        return 0;
    }
}