#pragma once

class Server
{
public:
    static int StartServer();
    static void ReceiveLoop(int socket_fd);
};

class Client
{
public:
    static int Connect();
};

