#pragma once
#include <string>
#include <winsock2.h>

class Socket {
public:
    Socket(SOCKET s);
    ~Socket();

    int sendData(const std::string& data);
    std::string receiveData();
	std::string getRemoteIp() const;
    void close();

private:
    SOCKET m_socket;
};
