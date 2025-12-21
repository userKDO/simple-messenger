#pragma once
#include <string>

class GUI
{
public:
    static std::string ip;
    static int port;

    static void AskIP();
    static void AskPort();
};

