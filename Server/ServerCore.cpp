#include "ServerCore.h"
#include "Socket.h"
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

Server::Server(int port) : m_port(port), m_running(false) {};


void Server::stop() {
    m_running = false;
    WSACleanup();
}

void Server::start() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(m_port);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (sockaddr*)&addr, sizeof(addr));
    listen(serverSocket, SOMAXCONN);

    std::cout << "Servidor escuchando en " << m_port << std::endl;

    while (true) {
        SOCKET clientSock = accept(serverSocket, nullptr, nullptr);
        std::cout << "Cliente conectado\n";

        Socket client(clientSock);

        while (m_running) {
            auto data = client.receiveData();
            if (data.empty()) break;

            std::cout << "Recibido: " << data << std::endl;
            client.sendData("ACK\r\n");
        }
    }
    closesocket(serverSocket);
}