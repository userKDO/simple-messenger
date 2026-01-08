#pragma once
#include <string>

bool client_run(const char* ip, int port);
bool sendMessage(int sock, const std::string& msg);

