#include "client.h"
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>


bool sendMessage(int sock, const std::string& msg)
{
   	uint32_t len = htonl(msg.size());

   	// send length
   	if (send(sock, &len, sizeof(len), 0) != sizeof(len)) {
       		return false;
	}

   	// send payload
	size_t sent = 0;
	while (sent < msg.size()) {
        	ssize_t s = send(sock, msg.data() + sent, msg.size() - sent, 0);
		if (s <= 0){
            		return false;
		}
        sent += s;
    }
    return true;
}

bool client_run(const char* ip, int port)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	std::string msg;
	
	if (sock < 0) {
		perror("client socket");
		return false;
	}


	sockaddr_in addr{};
   	addr.sin_family = AF_INET;
    	addr.sin_port = htons(port);
    	addr.sin_addr.s_addr = inet_addr(ip);

	while(connect(sock, (sockaddr*)&addr, sizeof(addr)) < 0) {
		std::cout << "[SYS] Waiting for server...\n";
		sleep(1);
	}
	
	std::cout << "[SYS] Connected\n";

	while(true) {
		std::cout << "> ";
		std::getline(std::cin, msg);

		if (msg == "/exit") {
			break;
		}

		sendMessage(sock, msg);
	}

	close(sock);
	return true;
}
