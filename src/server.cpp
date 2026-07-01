#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "../include/Database.h"

#define PORT 6379
#define BUFFER_SIZE 1024

Database db;

void handleClient(int clientSocket) {
    char buffer[BUFFER_SIZE];
    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytesRead = read(clientSocket, buffer, BUFFER_SIZE - 1);
        
        if (bytesRead <= 0) {
            std::cout << "Client disconnected.\n";
            close(clientSocket);
            break;
        }

        std::string input(buffer);
        // Clean up trailing newlines from client
        if (!input.empty() && input.back() == '\n') input.pop_back();

        std::string response = db.processCommand(input) + "\n";
        send(clientSocket, response.c_str(), response.length(), 0);
    }
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Failed to create socket\n";
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Bind failed\n";
        return 1;
    }

    listen(serverSocket, 5);
    std::cout << "AegisKV Server running on port " << PORT << "...\n";

    while (true) {
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
        if (clientSocket >= 0) {
            std::cout << "New client connected!\n";
            std::thread(handleClient, clientSocket).detach(); // Spawn thread for concurrency
        }
    }
    
    close(serverSocket);
    return 0;
}