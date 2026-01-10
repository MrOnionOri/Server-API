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
	ThreadPool pool;

public:
	Server(int port, Router* router);
	void start();
	void stop();
	void clientHandler(SOCKET clientSocket);

	ThreadPoolSnapshot getPoolSnapshot() const;
};