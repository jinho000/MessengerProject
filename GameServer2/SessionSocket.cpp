#include "pch.h"
#include "SessionSocket.h"

SessionSocket::SessionSocket()
	: Socket()
	, m_bUsed(false)
{
	// 소켓 옵션 설정
	
	// 네이글 알고리즘 끄기
	{
		BOOL bNoDelay = TRUE;
		int errorCode = setsockopt(m_socket
			, IPPROTO_TCP
			, TCP_NODELAY
			, reinterpret_cast<const char*>(&bNoDelay)
			, sizeof(BOOL)
		);

		if (SOCKET_ERROR == errorCode)
		{
			ServerHelper::PrintLastError("socket option error");
			CloseSocket();
			return;
		}
	}

	// 소켓 재활용 옵션
	{
		BOOL bReuse = TRUE;
		int errorCode = setsockopt(m_socket
			, SOL_SOCKET
			, SO_REUSEADDR
			, reinterpret_cast<const char*>(&bReuse)
			, sizeof(BOOL)
		);

		if (SOCKET_ERROR == errorCode)
		{
			ServerHelper::PrintLastError("socket option error");
			CloseSocket();
			return;
		}
	}

	// 연결관련 옵션
	{
		LINGER lin;
		lin.l_linger = 0;
		lin.l_onoff = 1;

		BOOL bReuse = TRUE;
		int errorCode = setsockopt(m_socket
			, SOL_SOCKET
			, SO_LINGER
			, reinterpret_cast<const char*>(&lin)
			, sizeof(LINGER)
		);

		if (SOCKET_ERROR == errorCode)
		{
			ServerHelper::PrintLastError("socket option error");
			CloseSocket();
			return;
		}
	}

}

void SessionSocket::SetClientAddress(SOCKADDR_IN _address)
{
	m_address = _address;

	// 클라이언트 정보 세팅
	char buff[255];
	inet_ntop(AF_INET, &m_address.sin_addr, buff, sizeof(buff));
	m_IP = std::string(buff);
	m_port = ntohs(m_address.sin_port);
}
