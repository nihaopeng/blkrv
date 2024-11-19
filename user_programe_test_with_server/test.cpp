#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define SERVER_IP "127.0.0.1" // 服务器 IP
#define SERVER_PORT 8080       // 服务器端口

class NetworkCard {
public:
    NetworkCard(const std::string& ip, int port)
        : server_ip(ip), server_port(port), sockfd(-1) {}

    ~NetworkCard() {
        if (sockfd != -1) {
            close(sockfd);
        }
    }

    void connectToServer() {
        // 创建 socket
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            perror("socket creation failed");
            return;
        }

        // 设置服务器地址结构
        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(server_port);
        inet_pton(AF_INET, server_ip.c_str(), &server_addr);

        // 连接到服务器
        if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            perror("connection to server failed");
            close(sockfd);
            sockfd = -1;
            return;
        }

        std::cout << "Connected to server: " << server_ip << ":" << server_port << std::endl;
    }

    void sendMessage(const std::string& message) {
        if (sockfd != -1) {
            send(sockfd, message.c_str(), message.size(), 0);
        }
    }

    std::string receiveMessage() {
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

    static void* threadFunction(void* arg) {
        NetworkCard* netCard = static_cast<NetworkCard*>(arg);
        netCard->connectToServer();

        // 发送和接收消息的循环
        while (true) {
            std::string msg;
            std::cout << "Enter message to send: ";
            std::getline(std::cin, msg);
            netCard->sendMessage(msg);

            std::string response = netCard->receiveMessage();
            if (!response.empty()) {
                std::cout << "Received: " << response << std::endl;
            }
        }
        return nullptr;
    }

    void start() {
        pthread_create(&thread, nullptr, threadFunction, this);
    }

private:
    std::string server_ip;
    int server_port;
    int sockfd;
    pthread_t thread;
};

int main() {
    NetworkCard netCard(SERVER_IP, SERVER_PORT);
    netCard.start();

    // 等待线程结束
    pthread_exit(nullptr);
    return 0;
}
