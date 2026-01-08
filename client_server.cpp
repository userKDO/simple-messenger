#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>

void receive(int port)
{
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htons(INADDR_ANY);

	int server_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (bind(server_socket, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		std::cout << "Socket error" << std::endl;
		return 1
	}

	if (listen(server_socket, 1) == 0) {
		int client;

		if ((client = accept (server, nullptr, nullptr)) < 0) {
			
}
