#include "session.h"
#include "server.h"
#include "../4dev/logger/logger.h"

#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <algorithm>

Session::Session(int sock, Server* server) // initializes:
										  // sock - client's file desryptor
										  // server - server pointer
	: sock(sock), server(server) {} // here is a NO THREADS

Session::~Session() // session destructor
{
	stop(); // guarantee that session is stopped before destruction
}

void Session::start() // Turns on alive flag and starting the thread
{
	running = true;
	recvThread = std::thread(&Session::recvLoop, this);
}

void Session::stop()
{
	if (!running)
		return;

	running = false;
	shutdown(sock, SHUT_RDWR); // no more writing and no more reading
							   // this is unblocks the recv()
	close(sock);

	// IMPORTANT:
	// stop() can be called from outside recvThread,
	// so join is safe here
	if (recvThread.joinable())
		recvThread.join();
	// NOTE:
	// removeSession() is NOT called here anymore
	// it is called exactly once from recvLoop()
}

void Session::recvLoop()
{
    char temp[1024]; // buffer for recv()
    while (running)
    {
        int bytes = recv(sock, temp, sizeof(temp), 0); // reading bytes from TCP
                                                       // if recv > 0  - data
                                                       // if recv == 0 - client left
                                                       // if recv < 0  - error
        if (bytes <= 0)
        {
            server->logger().warning(
                "recv failed or client disconnected, fd=" + std::to_string(sock)
            );
            break;
        }

        buffer.insert(buffer.end(), temp, temp + bytes);

        while (processBuffer()) {}
    }

    running = false;
    server->removeSession(this);
}

// returns true if one full message was processed
bool Session::processBuffer()
{
	if (buffer.size() < 4)
		return false;

	uint32_t len;
	std::memcpy(&len, buffer.data(), 4);
	len = ntohl(len);

	if (buffer.size() < 4 + len)
		return false;

	std::string msg(buffer.begin() + 4, buffer.begin() + 4 + len);
	server->logger().info("Message received: " + msg);

	buffer.erase(buffer.begin(), buffer.begin() + 4 + len);
	return true;
}

