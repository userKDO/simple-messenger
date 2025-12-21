#pragma once

class Server
{
public:
    static int StartServer();
    static void receiveMsg(int socket_fd);
};

class Client
{
public:
    static int connectToPeer();
};

