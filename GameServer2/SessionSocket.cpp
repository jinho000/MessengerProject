#include "pch.h"
#include "SessionSocket.h"

SessionSocket::SessionSocket(SOCKET _socket, SOCKADDR_IN _clientAddress)
	: Socket(_socket, _clientAddress)
{

}
