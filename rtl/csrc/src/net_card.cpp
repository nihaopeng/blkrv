#include "net_card.h"

net_card::net_card(uint32_t size):vmem(size){
    this->if_start_up=0;
}

net_card::~net_card(){pthread_join(thread, nullptr);}

sok* net_card::sok_from_blkos(){
    while(1){
        if(this->get4B(20)){//sData_length is exist
            sok* sock_from_os=(sok*)malloc(sizeof(sok));
            for(int i=0;i<16;i++){
                sock_from_os->ip[i]=this->getB(i);
            }
            sock_from_os->port=this->get4B(16);
            sock_from_os->data_len=this->get4B(20);
            this->put4B(20,0);//清除标志
            sock_from_os->data=(char*)malloc(sizeof(char)*sock_from_os->data_len);
            for(int i=0;i<sock_from_os->data_len;i++){
                sock_from_os->data[i]=this->getB(24+i);
            }
            return sock_from_os;
        }
    }
}

void net_card::connect2server(char ip[16],uint32_t port) {
    // 创建 socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        return;
    }
    // 设置服务器地址结构
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);
    // 连接到服务器
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connection to server failed");
        close(sockfd);
        sockfd = -1;
        return;
    }
    // std::cout << "Connected to server: " << server_ip << ":" << server_port << std::endl;
}

void net_card::send_message(const char* message,const uint32_t data_len) {
    if (sockfd != -1) {
        send(sockfd, message, data_len, 0);
    }
}

std::string net_card::receive_message() {
    char buffer[1024] = {0};
    if (sockfd != -1) {
        ssize_t bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            return std::string(buffer);
        }
    }
    return "";
}

void* net_card::thread_function(void* arg) {
    std::cout<<"net interface card start up!\n";
    while(1){
        net_card* nic = static_cast<net_card*>(arg);
        sok* sock=nic->sok_from_blkos();//阻塞
        // std::cout<<"get a request!\nip:"<<sock->ip<<"port:"<<sock->port<<"\ndata:"<<sock->data;
        if(sock){
            nic->connect2server(sock->ip,sock->port);
            // 发送和接收消息的循环
            nic->send_message(sock->data,sock->data_len);
            std::string response = nic->receive_message();
            if (!response.empty()) {
                // std::cout << "Received: " << response << std::endl;
                uint32_t rData_addr=4*1024*1024+4;
                for(int l=0;l<response.size();l++){
                    nic->putB(rData_addr+l,response[l]);
                }
                nic->put4B(4*1024*1024,response.size());//put at last as a mark;
            }
        }
    }
    return nullptr;
}

void net_card::process(Vtop* top){
    if(!this->if_start_up){
        pthread_create(&thread,NULL,thread_function,this);
        this->if_start_up=1;
    }
    if(top->s5_req){
        if(top->s5_we){
            switch(top->s5_mem_op_type){
                case 0:this->putB(top->s5_addr,uint8_t(top->s5_write_data));break;
                case 1:this->put2B(top->s5_addr,uint16_t(top->s5_write_data));break;
                case 2:this->put4B(top->s5_addr,uint32_t(top->s5_write_data));
                        // printf("%d:%d:%d\n",top->s1_addr,top->s1_write_data,this->get4B(top->s1_addr));
                        break;
                default:break;
            }
        }else{
            switch(top->s5_mem_op_type){
                case 0:top->s5_read_data=uint8_t(this->getB(top->s5_addr));break;
                case 1:top->s5_read_data=uint16_t(this->get2B(top->s5_addr));break;
                case 2:top->s5_read_data=uint32_t(this->get4B(top->s5_addr));break;
                default:break;
            }
        }
    }
}