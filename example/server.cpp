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
                
                // 读取文件并准备响应
                std::string filePath(buffer);
                std::string response = readBinaryFile(filePath);
                uint32_t file_size = static_cast<uint32_t>(response.size());
                // uint32_t net_file_size = htonl(file_size); // 转换为网络字节序
                printf("File size: %u bytes\n", file_size);
                // 发送文件大小（4字节）
                ssize_t sent_size = send(new_socket, &file_size, sizeof(file_size), 0);
                if (sent_size != sizeof(file_size)) {
                    perror("send file size failed");
                    break;
                }

                // 发送文件内容
                const char* file_data = response.data();
                size_t remaining = file_size;
                while (remaining > 0) {
                    ssize_t sent_bytes = send(new_socket, file_data, remaining, 0);
                    if (sent_bytes <= 0) {
                        perror("send file data failed");
                        break;
                    }
                    remaining -= sent_bytes;
                    file_data += sent_bytes;
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
