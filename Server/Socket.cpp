#include "Socket.h"

Socket::Socket(SOCKET s) : m_socket(s){}

Socket::~Socket() {
	close();
}

int Socket::sendData(const std::string& data) {
	return send(m_socket, data.c_str(), (int)data.size(), 0);
}

std::string Socket::receiveData() {
	char buffer[1024];
	int bytes = recv(m_socket, buffer, sizeof(buffer), 0);

	if (bytes <= 0) return "";
	return std::string(buffer, bytes);
}

void Socket::close() {
	if (m_socket != INVALID_SOCKET) {
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	}
}