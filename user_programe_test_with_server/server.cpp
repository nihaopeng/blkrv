#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <fstream>
#include <string>

#define PORT 8080

std::string readBinaryFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (file) {
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);
        std::string buffer(size, '\0');
        if (file.read(&buffer[0], size)) {
            return buffer;
        }
    }
    return "";
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // 创建 socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 绑定 socket
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // 监听所有 IP
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 监听连接
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    // 接受连接
    while ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) >= 0) {
        std::cout << "New connection accepted." << std::endl;

        while (true) {
            memset(buffer, 0, sizeof(buffer));
            int bytes_read = read(new_socket, buffer, sizeof(buffer));
            if (bytes_read > 0) {
                std::cout << "Received: " << buffer << std::endl;
                
                // 回复客户端
                if(!strcmp(buffer,"testfile"))
                {
                    std::string response =readBinaryFile("./hello");
                    std::cout<<"size:"<<response.size()<<std::endl;
                    send(new_socket, response.c_str(), response.size(), 0);
                }
                else if(!strcmp(buffer,"hello")){
                    std::string response= "hello,this is response from icca";
                    send(new_socket, response.c_str(), response.size(), 0);
                }
                else if(!strcmp(buffer,"jpg")){
                    std::string response =readBinaryFile("./test.jpg");
                    std::cout<<"size:"<<response.size()<<std::endl;
                    send(new_socket, response.c_str(), response.size(), 0);
                }
                break;
            } else {
                std::cout << "Connection closed." << std::endl;
                break;
            }
        }
        close(new_socket);
    }

    return 0;
}
