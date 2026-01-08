#include "TCP/server.h"
#include <iostream>

int main()
{
	Server server(8080); // in the () is a port
	if (!server.start())
	{
		std::cerr << "Server start failed\n"; // here we can add shit if server failed
											  // for example: if server returns false >> Server server(9090)
		return 1;
	}

	server.run();
	return 0;
}
