#include <thread>
#include <atomic>
#include "client.h"
#include "server.h"
#include <unistd.h>

int main()
{ 
	constexpr int PORT = 8080;

	std::thread server_thread(server_run, PORT);
	
	sleep(1);
	
	std::thread client_thread(client_run, "127.0.0.1", PORT);
	
	server_thread.join();
	client_thread.join();
	return 0;
}
