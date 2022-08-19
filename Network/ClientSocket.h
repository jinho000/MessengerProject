#pragma once
#include "Socket.h"
#include <PacketLibrary/PacketBase.h>

class ClientSocket : public Socket
{
private:

public:
	ClientSocket(int _serverPort, std::string _serverIP, IPPROTO _protocol);
	~ClientSocket() = default;

public:
	int GetServerPort() const { return m_port; }
	std::string GetServerIP() const { return m_IP; }

public:
	bool ConnectServer();
};

