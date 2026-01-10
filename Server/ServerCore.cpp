#include "ServerCore.h"
#include "Socket.h"
#include "ThreadPool.h"
#include "HttpParser.h"
#include "ConsoleHandler.h"
#include <iostream>
#include <thread>

#pragma comment(lib, "Ws2_32.lib")
ThreadPool pool;

Server::Server(int port, Router* router)
    : m_port(port), m_running(false), router(router), pool(this, 5, 30) {}


void Server::stop() {
    m_running = false;
    pool.stop();
    if (m_serverSocket != INVALID_SOCKET) {
        closesocket(m_serverSocket);
        m_serverSocket = INVALID_SOCKET;
    }
    std::cout << "Servidor detenido señal enviada." << std::endl;
}

void Server::clientHandler(SOCKET clientSocket) {
    Socket client(clientSocket);

    auto raw = client.receiveData();
    if (raw.empty()) return;

    HttpRequest request = parseHttpRequest(raw);
    request.ipAddress = client.getRemoteIp();
	std::cout << "[REQUEST] IP: " << request.ipAddress << " " << request.method << " " << request.path << std::endl;

    HttpResponse response = router->route(request);

    client.sendData(response.toString());
}

void Server::start() {
    RegisterConsoleHandler(this);

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "WSAStartup failed\n";
        return;
    }

    m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        WSACleanup();
        return;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(m_port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(m_serverSocket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed\n";
        closesocket(m_serverSocket);
        WSACleanup();
        return;
    }

    if (listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed\n";
        closesocket(m_serverSocket);
        WSACleanup();
        return;
    }

    std::cout << "Servidor escuchando en " << m_port << std::endl;

    m_running = true;

	//pool.initialize(this, 5, 30);

    /*std::thread monitor([this, &pool]() {
        while (m_running) {
            auto stats = pool.getSnapshot();

            std::cout
                << "[POOL] Total: " << stats.totalThreads
                << " | Busy: " << stats.busyThreads
                << " | Idle: " << stats.idleThreads
                << " | Queue: " << stats.queuedTasks
                << std::endl;

            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
        });
    monitor.detach();*/

    while (m_running) {
        SOCKET clientSock = accept(m_serverSocket, nullptr, nullptr);

        if (clientSock == INVALID_SOCKET) {

            if (!m_running) {
                std::cout << "Server shutdown detected inside loop.\n";
                break;
            }
            continue;
        }

        pool.enqueue(clientSock);
    }

    pool.stop();
    closesocket(m_serverSocket);
    WSACleanup();
}

ThreadPoolSnapshot Server::getPoolSnapshot() const {
    return pool.getSnapshot();
}