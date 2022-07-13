#include "pch.h"
#include "ListenSocket.h"
#include "Socket.h"

ListenSocket::ListenSocket(int _serverPort, std::string _serverIP)
	: Socket(_serverPort, _serverIP, IPPROTO::IPPROTO_TCP)
{
}

void ListenSocket::StartListen()
{
	listen(m_socket, SOMAXCONN);

	// accept, acceptEx

}



