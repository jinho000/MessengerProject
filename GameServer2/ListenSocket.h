#pragma once
#include "Socket.h"


class ListenSocket : public Socket
{
private: // member var
	IOCompletionCallback m_listenCompleteCallback;

public: // default
	ListenSocket(int _serverPort, std::string _serverIP);
	~ListenSocket() = default;

private:
	void ListenCompleteCallback(DWORD _transferredBytes, LPOVERLAPPED _IOData);

public:
	int GetServerPort() const { return m_port; }
	std::string GetServerIP() const { return m_IP; }

public:
	void StartListen();
	void RequestAsyncAccept();
};

