#pragma once
#include "Singleton.h"


class TCPSession;
class TCPSessionPool : public Singleton<TCPSessionPool>
{
	friend class Singleton;

private: // member var
	std::unordered_map<TCPSession*, TCPSession*>	m_TCPSessionPool;
	std::mutex										m_lock;

private:
	TCPSessionPool();
	~TCPSessionPool();

public:
	void PopTCPSession(TCPSession* _pTCPSession);
	void RetrieveTCPSession(TCPSession* _pTCPSession);

};

