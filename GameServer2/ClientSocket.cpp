#include "pch.h"
#include "ClientSocket.h"

ClientSocket::ClientSocket(int _serverPort, std::string _serverIP, IPPROTO _protocol)
	: Socket(_serverPort, _serverIP, _protocol)
{
}


bool ClientSocket::ConnectServer()
{
	if (m_protocol == IPPROTO::IPPROTO_UDP)
		return false;

	if (SOCKET_ERROR == connect(m_socket, reinterpret_cast<sockaddr*>(&m_address), sizeof(m_address)))
	{
		ServerHelper::Log("Connect Server Fail");
		return false;
	}

	return true;
}
