#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>

#define BUFFER_SIZE 2048
#define PORT 8080

void server_connect(char *server_ip, int socket_fd) {

	sockaddr_in addr;
 	addr.sin_family = AF_INET;
 	addr.sin_port = htons(PORT); 
 	addr.sin_addr.s_addr = inet_addr(server_ip);

	std::cout << server_ip << std::endl;

	if(connect(socket_fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		std::cout << "I can't connect" << std::endl;
	}
	else {
		std::cout << "I connect" << std::endl;
	}

}

void server_message() {

}

void in_chat(int socket_fd, std::string buffer) {
	while (1) {
		std::cout << "> ";
		getline(std::cin, buffer);
		if (buffer == "/close") {
			break;
		}
		send(socket_fd, buffer.c_str(), buffer.size(),0);
	}
}

int main()
{
	char serv[] = "127.0.0.1";
	int socket_fd;
	std::string command;
	std::string buffer;
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	
	std::cout << "Welcome" << std::endl;

	while (1) {
		std::cout << "[Menu] > ";
 		getline(std::cin, buffer);
		if (buffer == "/exit") {
			break;
		}
		else if (buffer == "/connect") {
			server_connect(serv, socket_fd);
			in_chat(socket_fd, buffer);
		}
		else {
			std::cout << "Please, write somthing normal" << std::endl;
		}
	}
  	return 0;
}
