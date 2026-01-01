#pragma once
class Server {
private:
	int m_port;
	bool m_running;
public:
	Server(int port);
	void start();
	void stop();
};