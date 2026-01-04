#include "Socket.h"
#include <ws2tcpip.h>


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

std::string Socket::getRemoteIp() const {
	sockaddr_in addr{};
	int len = sizeof(addr);
	char ipStr[INET_ADDRSTRLEN];

	if (getpeername(m_socket, reinterpret_cast<sockaddr*>(&addr), &len) == 0) {
		InetNtopA(AF_INET, &addr.sin_addr, ipStr, INET_ADDRSTRLEN);
		return std::string(ipStr);
	}
	return "";
}
