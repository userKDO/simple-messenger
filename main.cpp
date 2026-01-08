#include <thread>
#include "client.h"
#include "server.h"

int main()
{
	constexpr int PORT = 8080;

	std::thread server_thread(server_run, PORT);

	sleep(1);

	client_run("127.0.0.1", PORT);

	server_thread.join();
	return 0;
}
