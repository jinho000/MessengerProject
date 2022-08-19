#pragma once
#include "Singleton.h"

class SessionManger;
class TCPSession;
class SessionManager : public Singleton<SessionManager>
{
	friend class Singleton;

private: // default
	SessionManager();
	~SessionManager();

private:
	std::unordered_map<TCPSession*, TCPSession*>	m_connectedTCPSession;
	std::mutex										m_lock;

public:
	void AddTCPSession(TCPSession* _pTCPSession);
	void PopTCPSession(TCPSession* _key);
};

