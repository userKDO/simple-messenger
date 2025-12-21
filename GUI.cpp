#include "GUI.h"
#include <iostream>

std::string GUI::ip;
int GUI::port;

void GUI::AskIP()
{
    std::cout << "Enter IP: ";
    std::cin >> ip;
}

void GUI::AskPort()
{
    std::cout << "Enter port: ";
    std::cin >> port;
}

