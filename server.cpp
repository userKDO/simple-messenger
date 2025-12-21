#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>

#define BUFFER_SIZE 2048

void recieve_message()
{
	
}

int main(int argc, char *argv[])
{
	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
    	addr.sin_family = AF_INET;
    	addr.sin_port = htons(8080);

	addr.sin_addr.s_addr = INADDR_ANY;
	
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (bind(socket_fd, (sockaddr*)&addr,sizeof(addr)) < 0) {
		std::cout << "Port error" << std::endl;
		return 1;
	}
	else {
		std::cout << "Port is open" << std::endl;
	}

	if (listen(socket_fd,2) == 0) {
		std::cout << "Listen..." << std::endl;
		int client;
		if ((client = accept(socket_fd, nullptr, nullptr)) < 0) {
			std::cout << "can't make the connection" << std::endl;
			close(socket_fd);
			return -1;
		}
		else {
			std::cout << "Succes" << std::endl;
		
			char buffer[BUFFER_SIZE];
			int bytes_read = recv(client, buffer, BUFFER_SIZE, 0);
			std::cout << buffer << std::endl;
			close(client);
			return 0;
		}
	}
}
