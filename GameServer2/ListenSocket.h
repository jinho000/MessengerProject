#pragma once
#include "Socket.h"


class ListenSocket : public Socket
{
public: // default
	ListenSocket(int _serverPort, std::string _serverIP);
	~ListenSocket();

public:
	int GetServerPort() const { return m_port; }
	std::string GetServerIP() const { return m_IP; }
};

