#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 6379
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error\n";
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address\n";
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection Failed. Is the server running?\n";
        return -1;
    }

    std::cout << "Connected to AegisKV Server.\n";
    std::string input;

    while (true) {
        std::cout << "127.0.0.1:6379> ";
        std::getline(std::cin, input);
        
        if (input == "EXIT" || input == "QUIT") break;
        if (input.empty()) continue;

        input += "\n";
        send(sock, input.c_str(), input.length(), 0);
        
        memset(buffer, 0, BUFFER_SIZE);
        read(sock, buffer, BUFFER_SIZE);
        std::cout << buffer;
    }

    close(sock);
    return 0;
}