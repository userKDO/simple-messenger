#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define BUFFER_SIZE 2048

void server_connect(char *server_ip, int socket_fd) {

	sockaddr_in addr;
 	addr.sin_family = AF_INET;
 	addr.sin_port = htons(8080); 
 	addr.sin_addr.s_addr = inet_addr(server_ip);

	std::cout << server_ip << std::endl;

	if(connect(socket_fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		std::cout << "I can't connect" << std::endl;
	}
	else {
		std::cout << "I connect" << std::endl;
	}

}

int main(int argc, char *argv[])
{
	int socket_fd;
	std::string command;
	std::string buffer;
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	char serv[13] = "127.0.0.1";
	
	std::cout << "Hello" << std::endl;

	while (1) {
 		getline(std::cin, buffer);
		if (buffer == "/exit") {
			break;
		}
		else if (buffer == "/connect") {
			server_connect(serv, socket_fd);
		}
		else {
 			std::cout << "Отправил ваше сообщение: " << buffer << std::endl;
 			send(socket_fd, buffer.c_str(), buffer.size(), 0);
		}
	}
  	return 0;
}
