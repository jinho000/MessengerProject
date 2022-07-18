#pragma once
#include "Socket.h"

class SessionSocket : public Socket
{
private:
	bool m_bRegistIOCP; // IOCP에 등록된 소켓인지 여부

public:
	SessionSocket();
	~SessionSocket() = default;

public:
	void SetClientAddress(SOCKADDR_IN _address);
	void SetRegistIOCP() { m_bRegistIOCP = true; }

	int GetClientPort() const { return m_port; }
	std::string GetClientIP() const { return m_IP; }
	bool IsRegistIOCP() { return m_bRegistIOCP; }

};
