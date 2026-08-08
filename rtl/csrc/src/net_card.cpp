#include "net_card.h"
#include <fcntl.h>
#include <errno.h>
#include <cstring>
#include <cstdio>

net_card::net_card(uint32_t size):vmem(size){
    this->pending_sockfd=-1;
    this->pending_connect_fd=-1;
    this->pending_send=nullptr;
    this->pending_send_len=0;
    this->pending_send_off=0;
    this->pending_recv=nullptr;
    this->pending_recv_len=0;
    this->pending_recv_off=0;
    this->irq_pending=false;
}

net_card::~net_card(){
    if(this->pending_send) delete[] this->pending_send;
    if(this->pending_recv) delete[] this->pending_recv;
}

// 非阻塞 accept: 无客户端连接时保持监听状态, 每 tick 重试
int net_card::blk_accept_nb(){
    if(this->pending_sockfd<0){//新命令, 建立监听
        char ip[16];
        for(int i=4;i<20;i++){
            ip[i-4]=this->getB(i);
        }
        ip[15]=0;
        uint16_t port=this->get4B(20);
        int server_fd=socket(AF_INET, SOCK_STREAM, 0);
        if(server_fd<0) return 0;
        int flags=fcntl(server_fd, F_GETFL, 0);
        fcntl(server_fd, F_SETFL, flags|O_NONBLOCK);
        struct sockaddr_in server_addr;
        server_addr.sin_family=AF_INET;
        server_addr.sin_addr.s_addr=inet_addr(ip);
        server_addr.sin_port=htons(port);
        if(bind(server_fd,(struct sockaddr*)&server_addr,sizeof(server_addr))<0){
            close(server_fd);
            return 0;
        }
        listen(server_fd, 5);
        this->pending_sockfd=server_fd;
    }
    struct sockaddr_in client_addr;
    socklen_t addr_len=sizeof(client_addr);
    int client_fd=accept(this->pending_sockfd,(struct sockaddr*)&client_addr,&addr_len);
    if(client_fd<0){
        // EAGAIN: 尚无客户端, 保持监听, 下个 tick 再试
        return 0;
    }
    int flags=fcntl(client_fd, F_GETFL, 0);
    fcntl(client_fd, F_SETFL, flags|O_NONBLOCK);
    this->put4B(28,client_fd);//通知连接完成
    this->put4B(0,0);
    this->irq_pending=true;
    close(this->pending_sockfd);
    this->pending_sockfd=-1;
    return 0;
}

// 非阻塞 connect: EINPROGRESS 后通过 getsockopt 轮询连接结果
int net_card::blk_connect_nb(){
    if(this->pending_connect_fd<0){//新命令
        char ip[16]={0};
        for(int i=4;i<20;i++){
            ip[i-4]=this->getB(i);
        }
        uint16_t port=this->get4B(20);
        int sockfd=socket(AF_INET, SOCK_STREAM, 0);
        if(sockfd<0) return 0;
        int flags=fcntl(sockfd, F_GETFL, 0);
        fcntl(sockfd, F_SETFL, flags|O_NONBLOCK);
        struct sockaddr_in servaddr;
        servaddr.sin_family=AF_INET;
        servaddr.sin_port=htons(port);
        inet_pton(AF_INET, ip, &servaddr.sin_addr);
        int ret=connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
        if(ret==0){//立即成功
            this->put4B(28,sockfd);
            this->put4B(0,0);
            this->irq_pending=true;
            return 0;
        }
        if(errno!=EINPROGRESS){//连接失败
            close(sockfd);
            this->put4B(28,-1);
            this->put4B(0,0);
            this->irq_pending=true;
            return 0;
        }
        this->pending_connect_fd=sockfd;
        return 0;
    }
    int err=0;
    socklen_t len=sizeof(err);
    getsockopt(this->pending_connect_fd, SOL_SOCKET, SO_ERROR, &err, &len);
    if(err!=0) return 0;//尚未连接成功, 下个 tick 再查
    this->put4B(28,this->pending_connect_fd);//通知连接完成
    this->put4B(0,0);
    this->irq_pending=true;
    this->pending_connect_fd=-1;
    return 0;
}

// 非阻塞 send: 数据经总线 DMA 读入缓冲, 分段发送直到 EAGAIN 或发完
int net_card::blk_send_nb(){
    int sockfd=this->get4B(4);
    int data_phy_addr=this->get4B(8);
    int data_len=this->get4B(12);
    if(this->pending_send==nullptr){//新命令, DMA 读入发送缓冲
        this->pending_send=new char[data_len];
        this->pending_send_len=data_len;
        this->pending_send_off=0;
        // 按字批量读, 减少总线访问次数
        int i=0;
        int aligned_end=data_len & ~3;
        for(;i<aligned_end;i+=4){
            uint32_t w=this->bus->read(data_phy_addr+i,2);
            this->pending_send[i]=w&0xff;
            this->pending_send[i+1]=(w>>8)&0xff;
            this->pending_send[i+2]=(w>>16)&0xff;
            this->pending_send[i+3]=(w>>24)&0xff;
        }
        for(;i<data_len;i++){
            this->pending_send[i]=this->bus->read(data_phy_addr+i,0);
        }
    }
    int n=send(sockfd, this->pending_send+this->pending_send_off,
               this->pending_send_len-this->pending_send_off, 0);
    if(n<0){
        if(errno==EAGAIN||errno==EWOULDBLOCK) return 0;//下个 tick 继续
        delete[] this->pending_send;
        this->pending_send=nullptr;
        this->put4B(0,0);
        this->irq_pending=true;
        return 0;
    }
    this->pending_send_off+=n;
    if(this->pending_send_off>=this->pending_send_len){
        delete[] this->pending_send;
        this->pending_send=nullptr;
        this->put4B(0,0);//通知发送完成
        this->irq_pending=true;
    }
    return 0;
}

// 非阻塞 recv: 分段接收, 收满后经总线 DMA 写入内存
void net_card::deliver_recv(){
    int data_phy_addr=this->get4B(8);
    int got=this->pending_recv_off;
    int i=0;
    int aligned_end=got & ~3;
    for(;i<aligned_end;i+=4){
        uint32_t w=(uint8_t)this->pending_recv[i]|((uint8_t)this->pending_recv[i+1]<<8)
                   |((uint8_t)this->pending_recv[i+2]<<16)|((uint8_t)this->pending_recv[i+3]<<24);
        this->bus->write(data_phy_addr+i,w,2);
    }
    for(;i<got;i++){
        this->bus->write(data_phy_addr+i,this->pending_recv[i],0);
    }
    this->put4B(16,got);//通知接收数据长度
    delete[] this->pending_recv;
    this->pending_recv=nullptr;
    this->pending_recv_off=0;
    this->put4B(0,0);//通知接收完成
    this->irq_pending=true;
}

int net_card::blk_recv_nb(){
    int sockfd=this->get4B(4);
    int data_phy_addr=this->get4B(8);
    int data_len=this->get4B(12);
    if(this->pending_recv==nullptr){//新命令, 分配接收缓冲
        // fprintf(stderr,"[net] recv cmd sock=%d len=%d\n", sockfd, data_len);
        this->pending_recv=new char[data_len];
        this->pending_recv_len=data_len;
        this->pending_recv_off=0;
    }
    int n=recv(sockfd, this->pending_recv+this->pending_recv_off,
               this->pending_recv_len-this->pending_recv_off, 0);
    if(n<0){
        if(errno==EAGAIN||errno==EWOULDBLOCK){
            // fprintf(stderr,"[net] recv EAGAIN off=%d remain=%d\n", this->pending_recv_off, this->pending_recv_len-this->pending_recv_off);
            return 0;//下个 tick 继续
        }
        // fprintf(stderr,"[net] recv ERROR errno=%d\n", errno);
        if(this->pending_recv_off>0){
            // 出错前已收到部分数据: 交付已收部分, 避免内核误以为对端关闭
            this->deliver_recv();
        }else{
            this->put4B(16,0);
            this->put4B(0,0);
            this->irq_pending=true;
        }
        return 0;
    }
    if(n==0){//对端关闭
        // fprintf(stderr,"[net] recv CLOSED off=%d remain=%d\n", this->pending_recv_off, this->pending_recv_len-this->pending_recv_off);
        if(this->pending_recv_off>0){
            // 服务端发完剩余数据后关闭: 把已收到的部分交付内核 (标准 recv 语义:
            // 最后一次返回剩余字节, 下一次再返回 0)
            this->deliver_recv();
        }else{
            this->put4B(16,0);
            this->put4B(0,0);
            this->irq_pending=true;
        }
        return 0;
    }
    this->pending_recv_off+=n;
    // fprintf(stderr,"[net] recv got=%d off=%d/%d\n", n, this->pending_recv_off, this->pending_recv_len);
    if(this->pending_recv_off>=this->pending_recv_len){//收满, DMA 写入内存
        this->deliver_recv();
    }
    return 0;
}

int net_card::blk_close(){
    int sockfd=this->get4B(4);
    if(sockfd>=0) close(sockfd);
    this->put4B(0,0);//通知关闭完成
    this->irq_pending=true;
    return 0;
}

int net_card::process(Bus* bus,uint32_t tick){
    bus->set_irq(5, false);
    if(this->irq_pending){
        bus->set_irq(5, true);
        this->irq_pending = false;
    }
    switch(this->get4B(0)){//命令寄存器, 0=空闲
        case 1:this->blk_accept_nb();break;
        case 2:this->blk_send_nb();break;
        case 3:this->blk_recv_nb();break;
        case 4:this->blk_connect_nb();break;
        case 5:this->blk_close();break;
        default:break;
    }
    return 0;
}
