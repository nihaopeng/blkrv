#include "net_card.h"

net_card::net_card(uint32_t size):vmem(size){
    this->if_start_up=0;
}

net_card::~net_card(){pthread_join(thread, nullptr);}

int net_card::blk_accept(){
    if(this->get4B(0)==1){//port位不为0，产生请求
        char ip[16];
        for(int i=4;i<20;i++){
            ip[i-4]=this->getB(i);
        }
        uint16_t port=this->get4B(20);
        // printf("accept:ip:%s,port:%d\n",ip,port);
        uint32_t protocol=this->get4B(24);
        int server_fd = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in server_addr, client_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr(ip);
        server_addr.sin_port = htons(port);
        bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
        listen(server_fd, 5);
        socklen_t addr_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
        // printf("accept:client_fd:%d\n",client_fd);
        this->put4B(28,client_fd);
        this->put4B(0,0);//通知发送完成
    }
    return 0;
}

int net_card::blk_send(){
    if(this->get4B(0)==2){//sockfd位不为0，产生请求
        int sockfd=this->get4B(4);
        int data_phy_addr=this->get4B(8);
        int data_len=this->get4B(12);
        printf("send:sockfd:%d,data_phy_addr:%x,data_len:%d\n",sockfd,data_phy_addr,data_len);
        char* data=new char[data_len];
        for(int i=0;i<data_len;i++){
            data[i]=this->my_ram->getB(data_phy_addr+i);
        }
        printf("send data:%s\n",data);
        send(sockfd, data, data_len, 0);
        delete[] data;
        this->put4B(0,0);//通知发送完成
    }
    return 0;
}

int net_card::blk_recv(){
    if(this->get4B(0)==3){//sockfd位不为0，产生请求
        int sockfd=this->get4B(4);
        int data_phy_addr=this->get4B(8);
        int data_len=this->get4B(12);
        // printf("recv:sockfd:%d,data_phy_addr:%x,data_len:%d\n",sockfd,data_phy_addr,data_len);
        char* data=new char[data_len];
        uint32_t size=recv(sockfd, data, data_len, 0);
        for(int i=0;i<data_len;i++){
            this->my_ram->putB(data_phy_addr+i,data[i]);
        }
        delete[] data;
        this->put4B(16,size);//通知接收数据长度
        this->put4B(0,0);//通知接收完成
    }
    return 0;
}

int net_card::blk_connect(){
    if(this->get4B(0)==4){//port位不为0，产生请求
        char ip[16]={0};
        for(int i=4;i<20;i++){
            ip[i-4]=this->getB(i);
        }
        uint16_t port=this->get4B(20);
        uint32_t protocol=this->get4B(24);
        // printf("connect:ip:%s,port:%d\n",ip,port);
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in servaddr;
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(port);
        inet_pton(AF_INET, ip, &servaddr.sin_addr);
        connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
        // printf("connect:sockfd:%d\n",sockfd);
        this->put4B(28,sockfd);//通知连接完成
        this->put4B(0,0);//通知发送完成
    }
    return 0;
}

int net_card::blk_close(){
    if(this->get4B(0)==5){//sockfd位不为0，产生请求
        int sockfd=this->get4B(4);
        close(sockfd);
        this->put4B(0,0);//通知关闭完成
    }
    return 0;
}

void* net_card::thread_function(void* arg) {
    std::cout<<"net interface card start up!\n";
    net_card* nic = static_cast<net_card*>(arg);
    while(1){
        nic->blk_accept();
        nic->blk_connect();
        nic->blk_send();
        nic->blk_recv();
        nic->blk_close();
    }
    return nullptr;
}

int net_card::process(rib* rib,uint32_t tick){
    if(!this->if_start_up){
        pthread_create(&thread,NULL,thread_function,this);
        this->if_start_up=1;
    }
    if(rib->s5_req){
        if(rib->s5_we){
            switch(rib->s5_mem_op_type){
                case 0:this->putB(rib->s5_addr,uint8_t(rib->s5_write_data));break;
                case 1:this->put2B(rib->s5_addr,uint16_t(rib->s5_write_data));break;
                case 2:this->put4B(rib->s5_addr,uint32_t(rib->s5_write_data));
                        // printf("%x:%d:%d\n",rib->s5_addr,rib->s5_write_data,this->get4B(rib->s5_addr));
                        break;
                default:break;
            }
        }else{
            switch(rib->s5_mem_op_type){
                case 0:rib->s5_read_data=uint8_t(this->getB(rib->s5_addr));break;
                case 1:rib->s5_read_data=uint16_t(this->get2B(rib->s5_addr));break;
                case 2:rib->s5_read_data=uint32_t(this->get4B(rib->s5_addr));break;
                default:break;
            }
        }
    }
    return 0;
}

int net_card::dma_link(ram *my_ram)
{
    this->my_ram=my_ram;
    return 0;
}
