#pragma once
#include <vector>
#include <memory>
#include <thread>
#include <atomic>
#include <mutex>
#include "../4dev/logger/logger.h"

class Session;

class Server
{
public:
	explicit Server(int port);
	~Server();

	SimpleLogger& logger();

	bool start(); // listen socket
	void run(); // accept() cycle
	void removeSession(Session* s); // removes dead session
	
private:
	SimpleLogger log;
	int port; // port
	int listenSock{-1}; // file descryptor listen socket
						// -1 means "Not created yet"
	std::atomic<bool> running {false}; // is server alive?

	std::thread acceptThread; // not used yet
	
	std::vector<std::shared_ptr<Session>> sessions; // list of all sessions
													// shared_ptr:
													// server own session
													// thread of session owns too
	std::mutex sessionsMutex; // protects sessions from another threads
};
