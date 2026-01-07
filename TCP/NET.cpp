#include "NET.h"
#include "../GUI.h"
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

int Server::StartServer()
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("socket");
        return -1;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(GUI::port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (sockaddr*)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        close(server_fd);
        return -1;
    }

    if (listen(server_fd, 1) < 0)
    {
        perror("listen");
        close(server_fd);
        return -1;
    }

    std::cout << "Waiting for connection on port " << GUI::port << "...\n";

    int client_fd = accept(server_fd, nullptr, nullptr);
    if (client_fd < 0)
    {
        perror("accept");
        close(server_fd);
        return -1;
    }

    close(server_fd);
    return client_fd;
}

int Client::Connect()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket");
        return -1;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(GUI::port);
    inet_pton(AF_INET, GUI::ip.c_str(), &addr.sin_addr);

    if (connect(sock, (sockaddr*)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        close(sock);
        return -1;
    }

    return sock;
}

void Server::ReceiveLoop(int socket_fd)
{
    char buffer[1024];

    while (true)
    {
        int bytes = recv(socket_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0)
            break;

        buffer[bytes] = '\0';

        std::cout << "\rFriend: " << buffer << std::endl;
        std::cout << "> ";
        std::cout.flush();
    }
}

