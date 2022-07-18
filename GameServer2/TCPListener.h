#pragma once
#include "Singleton.h"
#include "ListenSocket.h"

class TCPSession;
class TCPListener : public Singleton<TCPListener>
{
	friend class Singleton;

private: // member var
	ListenSocket			m_listenSocket;
	IOCompletionCallback	m_listenCompleteCallback;

	std::list<TCPSession*>	m_acceptingTCPSession;

	enum
	{
		InitialAcceptSocketCount = 1,
	};

private: 
	TCPListener();
	~TCPListener();

private:
	void ListenCompleteCallback(DWORD _transferredBytes, LPOVERLAPPED _IOData);

public:
	SOCKET GetListenSocket() { return m_listenSocket.GetSocket(); }

	void StartListen();

};

