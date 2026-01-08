#include "server.h"
#include "session.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <algorithm>

Server::Server(int port) 
	: log("4dev/logs/server.log"),
	port(port) 
{} // Constructor that initializes port

Server::~Server() // server destructor
{
	running = false; // shouts: "Server is dying!"
	if (listenSock >= 0)
		close(listenSock); // closes listen socket and clears the port
	log.info("Server shutting down");

}

SimpleLogger& Server::logger()
{
	return log;
}

bool Server::start()
{
	listenSock = socket(AF_INET, SOCK_STREAM, 0); // Creates TCP-socket
												  // AF_INET - IPv4
												  // SOCK_STREAM - TCP
	log.info("Server starting on port " + std::to_string(port));

	if (listenSock < 0)
		return false; // error checker

	int opt = 1;
	setsockopt(listenSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); // allows second turn of server
																		 // without this port can be "used" or "unavailable"

	sockaddr_in addr{}; // structure of address
						// {} is nullifier
	addr.sin_family = AF_INET; // IPv4
	addr.sin_addr.s_addr = INADDR_ANY; // accept from any IP
	addr.sin_port = htons(port); // port -> network byte order

	if (bind(listenSock, (sockaddr*)&addr, sizeof(addr)) < 0) // binds socket to port
		return false;

	if (listen(listenSock, 16) < 0) // transfer to standby mode
									// 16 is a queue
		return false;

	running = true;
	return true; // socket is ready we can accept the clients
}

void Server::run()
{
	std::cout <<"Server listening on port " << port << "\n";
	log.info("Server listening");
	while (running)
	{
		int client = accept(listenSock, nullptr, nullptr); // While server alive
														   // Server is blocked
														   // waiting for client
														   // and return NEW socket

		if (client < 0)
			continue;

		log.info("Client connected, fd=" + std::to_string(client));

		auto session = std::make_shared<Session>(client, this); // creating new Session object
																// returning client socket and server

		{
			std::lock_guard<std::mutex> lock(sessionsMutex); 
			sessions.push_back(session); // server now owns the session
		}

		session->start(); // starting client accept thread  (IMPORTANT: NOT CLIENT'S. CLIENT ACCEPT THREAD)
	}
}

void Server::removeSession(Session* s)
{
	std::lock_guard<std::mutex> lock(sessionsMutex); // locking the access, RAII - is blocks automatically

	sessions.erase
		(
			std::remove_if(sessions.begin(), sessions.end(),
				[s](const std::shared_ptr<Session>& ptr) {
					return ptr.get() == s;
					}),
			sessions.end()
		);
}


