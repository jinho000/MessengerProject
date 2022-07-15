#include "pch.h"
#include "Socket.h"

Socket::Socket(int _port, std::string _IP, IPPROTO _protocol)
	: m_socket(NULL)
	, m_address({})
	, m_port(_port)
	, m_IP(_IP)
	, m_protocol(_protocol)
{
	if (m_protocol == IPPROTO::IPPROTO_TCP)
	{
		m_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO::IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);
	}
	else
	{
		m_socket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO::IPPROTO_UDP, NULL, NULL, WSA_FLAG_OVERLAPPED);
	}

	if (INVALID_SOCKET == m_socket)
	{
		ServerHelper::PrintLastError("create socket error");
		return;
	}

	// 주소값, 포트 세팅
	m_address.sin_family = AF_INET;
	m_address.sin_port = htons(m_port);

	if (SOCKET_ERROR == inet_pton(AF_INET, m_IP.c_str(), &m_address.sin_addr))
	{
		CloseSocket();
		ServerHelper::PrintLastError("socket inet_pton error");
		return;
	}

	// 소켓에 주소체계 바인드
	if (SOCKET_ERROR == bind(m_socket, (const sockaddr*)&m_address, sizeof(m_address)))
	{
		CloseSocket();
		ServerHelper::PrintLastError("socket bind error");
		return;
	}
}

Socket::Socket()
	: m_address({})
	, m_port(0)
	, m_protocol(IPPROTO::IPPROTO_TCP)
{
	m_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO::IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == m_socket)
	{
		ServerHelper::PrintLastError("create socket error");
		return;
	}
}

Socket::~Socket()
{
	CloseSocket();
}

void Socket::CloseSocket()
{
	if (NULL != m_socket)
	{
		closesocket(m_socket);
		m_socket = NULL;
	}
}

