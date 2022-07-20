#pragma once
#include "Singleton.h"


class TCPSession;
class TCPSessionPool : public Singleton<TCPSessionPool>
{
	friend class Singleton;

private: // member var
	std::queue<TCPSession*>	m_TCPSessionPool;
	std::mutex				m_lock;

	enum 
	{
		INITIAL_SESSION_COUNT = 10,
		RECREATE_SESSION_COUNT = 10
	};

private:
	TCPSessionPool();
	~TCPSessionPool();

	void CreateTCPSession(int _TCPSessionCount);

public:
	TCPSession* GetTCPSession();
	void RetrieveTCPSession(TCPSession* _pTCPSession);

};

