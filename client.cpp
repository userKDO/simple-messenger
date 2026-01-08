#include <iostream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

bool sendMessage(int sock, const std::string& msg)
{
    uint32_t len = htonl(msg.size());

    // send length
    if (send(sock, &len, sizeof(len), 0) != sizeof(len))
        return false;

    // send payload
    size_t total = 0;
    while (total < msg.size())
    {
        ssize_t sent = send(
            sock,
            msg.data() + total,
            msg.size() - total,
            0
        );

        if (sent <= 0)
            return false;

        total += sent;
    }

    return true;
}

int server_connect(int client_socket, char *server_ip, int port) // connect to server
{
	sockaddr_in addr;
 	addr.sin_family = AF_INET;
 	addr.sin_port = htons(port); 
 	addr.sin_addr.s_addr = inet_addr(server_ip);

	if(connect(client_socket, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		return 1;
	}
	else {
		std::cout << "Connection succsess" << std::endl;
		return 0;
	}
}

int main()
{
	char *server_ip = /*"127.0.0.1";*/"10.8.1.2";
	int port = 8080;
	int client_socket = socket(AF_INET, SOCK_STREAM, 0);
	std::string message;
	
	if (client_socket < 0) {
		std::cout << "Socket initialization error" << std::endl;
		return 1;
	}

	while(server_connect(client_socket, server_ip, port) > 0) {
		std::cout << "Can't connect to the server" << std::endl;
		sleep(2);
	}
	
	while(1) {
		std::cout << "> ";
		std::getline(std::cin, message);

		if (message == "/exit") {
			break;
		}
		else {
			sendMessage(client_socket, message);
		}
	}
	
	close(client_socket);
	return 0;
}
