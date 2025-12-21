#include <sys/socket.h>
#include <unistd.h>
#include "GUI.h"
#include "NET.h"
#include <thread>
#include <iostream>

int main()
{
    int choice;
    std::cout << "1 - server\n2 - client\n> ";
    std::cin >> choice;

    GUI::AskPort();

    int socket_fd;

    if (choice == 1)
    {
        socket_fd = Server::StartServer();
    }
    else
    {
        GUI::AskIP();
        socket_fd = Client::connectToPeer();
    }

    std::thread receiver(Server::receiveMsg, socket_fd);

    std::string msg;
    while (true)
    {
        std::getline(std::cin, msg);
        if (msg == "exit") break;
        send(socket_fd, msg.c_str(), msg.size(), 0);
    }

    receiver.join();
    close(socket_fd);
}

