#pragma once

class Socket
{
protected:
	SOCKET		m_socket;
	SOCKADDR_IN m_address;
	int			m_port;
	std::string	m_IP;
	IPPROTO		m_protocol;

public:
	Socket(int port, std::string _IP, IPPROTO _protocol);
	Socket(); // session socket에서 사용
	virtual ~Socket() = 0
	{
		CloseSocket();
	}

	Socket(const Socket& _other) = delete;
	Socket(Socket&& _other) = delete;
	Socket& operator=(const Socket& _other) = delete;
	Socket& operator=(const Socket&& _other) = delete;

protected:
	void CloseSocket();


public:
	// IPPROTO GetProtocol() const { return m_protocol; }

	SOCKET GetSocket() const { return m_socket; }

};

