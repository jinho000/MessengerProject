#pragma once
#include "Socket.h"
class SessionSocket : public Socket
{
private:

public:
	SessionSocket(SOCKET _socket, SOCKADDR_IN _clientAddress);
	~SessionSocket() = default;

public:
	int GetClientPort() const { return m_port; }
	std::string GetClientIP() const { return m_IP; }

};

