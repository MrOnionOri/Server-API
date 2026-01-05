#pragma once
#include "Router.h"
#include "ThreadPool.h"
#include "Socket.h"
#include <atomic>


class Server {
private:
	int m_port;
	std::atomic<bool> m_running;
	SOCKET m_serverSocket;
	Router* router;
	static Server* instance;

public:
	Server(int port);
	Server(int port, Router* router);
	void start();
	void stop();
	void clientHandler(int clientSocket);

};