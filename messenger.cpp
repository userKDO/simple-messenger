#include <sys/socket.h>
#include "GUI.h"
#include "TCP/NET.h"
#include <iostream>
#include <thread>
#include <unistd.h>
#include <limits>

int main()
{
    int choice;
    std::cout << "1 - server\n2 - client\n> ";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    GUI::AskPort();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    int socket_fd = -1;

    if (choice == 1)
    {
        socket_fd = Server::StartServer();
    }
    else
    {
        GUI::AskIP();
		//std::cout; //
        socket_fd = Client::Connect();
    }

    if (socket_fd < 0)
    {
        std::cerr << "Connection failed\n";
        return 1;
    }

    std::thread receiver(Server::ReceiveLoop, socket_fd);

    std::string msg;
	while (true)
	{
		std::cout << "> ";
		std::cout.flush();

		std::getline(std::cin, msg);

		if (msg == "exit")
			break;

		if (msg.empty())
			continue;

		if (send(socket_fd, msg.c_str(), msg.size(), 0) < 0)
		{
			perror("send");
			break;
		}
	}


    close(socket_fd);
    receiver.join();
    return 0;
}

