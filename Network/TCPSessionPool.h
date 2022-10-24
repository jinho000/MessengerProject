#pragma once
#include "Singleton.h"
#include "ObjectPool.h"

class TCPSession;
class TCPSessionPool : public Singleton<TCPSessionPool>
{
	friend class Singleton;

private: // member var
	std::unordered_map<TCPSession*, TCPSession*>	m_TCPSessionPool;
	ObjectPool<TCPSession>							m_TCPSessionObjPool;
	std::mutex										m_lock;

private:
	TCPSessionPool();
	~TCPSessionPool();

public:
	void PopTCPSession(TCPSession* _pTCPSession);
	void RetrieveTCPSession(TCPSession* _pTCPSession);

};

