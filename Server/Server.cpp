#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "ServerCore.h";

#pragma comment(lib, "Ws2_32.lib")

int main() {
    Server server(8080);
    server.start();
}
