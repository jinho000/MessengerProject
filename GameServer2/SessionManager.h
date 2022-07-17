#pragma once
#include "Singleton.h"

class SessionManger;
class SessionSocket;
class SessionManager : public Singleton<SessionManager>
{
	friend class Singleton;

private: // default
	SessionManager();
	~SessionManager();

private:
	std::list<SessionSocket*>	m_socketList;
	std::mutex					m_lock;

public:
	void AddSessionSocket(SessionSocket* _pSessionSocket);

};

