#pragma once
#include <thread>
#include <atomic>
#include <vector>

class Server; // Forward declaration like in other .h files

class Session
{
public:
	Session(int sock, Server* server); // constructor
									   // recieves: socket - client's socket
									   // server - server that created this session
	~Session(); // destructor

	void start(); // starts the thread for data recieve
	void stop();
	
private:
	void recvLoop();
	bool processBuffer();

	int sock; // TCP-socket of the client
			  // THIS IS NOT THE LISTEN SOCKET
	Server* server; // server pointer
	std::thread recvThread; // thread where recvLoop works
							// one thread - one client
	std::atomic<bool> running{false}; // alive flag of the session
									  // true - client connected
									  // false - time to cancel session
									  // atomic - cuz reading in one thread and writing in another one

	std::vector<char> buffer; // buffer for data
							  // TCP is a byte thread, not message

};
