#pragma once
#include "Socket.h"

class SessionSocket : public Socket
{
private:
	bool m_bRegistIOCP; // IOCP�� ��ϵ� �������� ����

public:
	SessionSocket();
	~SessionSocket() = default;

public:
	void SetClientAddress(SOCKADDR_IN _address);
	void SetRegistIOCP() { m_bRegistIOCP = true; }

	const int GetClientPort() const { return m_port; }
	const std::string& GetClientIP() const { return m_IP; }
	bool IsRegistIOCP() { return m_bRegistIOCP; }

};
