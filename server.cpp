#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>
#include <vector>
#include <thread>
#include <mutex>

#define BUFFER_SIZE 2048
#define PORT 8080

void recieve_message(int client)
{
	char buffer[BUFFER_SIZE];
	while (1) {
		std::memset(buffer,0,sizeof(buffer));
		int bytes_read = recv(client, buffer, BUFFER_SIZE, 0);
		std::string message(buffer,bytes_read);
		if (message == "/bye") {
			close(client);
			break;
		}
		std::cout << message << std::endl;
		
	}
}

void send_message(int client) {

}

int main()
{
	sockaddr_in addr;
	std::memset(&addr, 0, sizeof(addr));
    	addr.sin_family = AF_INET;
    	addr.sin_port = htons(PORT);

	addr.sin_addr.s_addr = INADDR_ANY;
	
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (bind(socket_fd, (sockaddr*)&addr,sizeof(addr)) < 0) {
		std::cout << "Port error" << std::endl;
		return 1;
	}
	else {
		std::cout << "Port " << PORT << " is open" << std::endl;
	}

	if (listen(socket_fd,2) == 0) {
		std::cout << "Listening..." << std::endl;
		int client;
		if ((client = accept(socket_fd, nullptr, nullptr)) < 0) {
			std::cout << "can't make the connection" << std::endl;
			close(socket_fd);
			return -1;
		}
		else {
			std::cout << "Connect " << client << std::endl;
		
			recieve_message(client);
			close(socket_fd);
			return 0;
		}
	}
}
