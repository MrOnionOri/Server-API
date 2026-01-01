#pragma once
#include "Router.h"

class Server {
private:
	int m_port;
	bool m_running;
	Router* router;
public:
	Server(int port);
	Server(int port, Router* router);
	void start();
	void stop();
	void clientHandler(int clientSocket);
};