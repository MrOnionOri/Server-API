#pragma once
class Server {
private:
	int m_port;
	bool m_running;
	void clientHandler(int clientSocket);
public:
	Server(int port);
	void start();
	void stop();
};