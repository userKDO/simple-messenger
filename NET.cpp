#include "NET.h"
#include "GUI.h"
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

int Server::StartServer()
{
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(GUI::port);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (sockaddr*)&addr, sizeof(addr));
    listen(server_socket, 1);

    std::cout << "Waiting for connection...\n";
    int client_socket = accept(server_socket, nullptr, nullptr);

    close(server_socket);
    return client_socket;
}

void Server::receiveMsg(int socket_fd)
{
    char buffer[1024];

    while (true)
    {
        int bytes = recv(socket_fd, buffer, sizeof(buffer), 0);
        if (bytes <= 0) break;

        buffer[bytes] = '\0';
        std::cout << "\nFriend: " << buffer << "\n> ";
        std::cout.flush();
    }
}

int Client::connectToPeer()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(GUI::port);
    inet_pton(AF_INET, GUI::ip.c_str(), &addr.sin_addr);

    connect(sock, (sockaddr*)&addr, sizeof(addr));
    return sock;
}

