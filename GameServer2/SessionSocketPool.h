#pragma once
#include "Singleton.h"

class SessionSocket;
class SessionSocketPool : public Singleton<SessionSocketPool>
{
	friend class Singlton;

private: // default
	SessionSocketPool();
	~SessionSocketPool();

private:
	std::queue<SessionSocket*>	m_sessionSocketPool;
	std::mutex					m_lock;

	enum
	{
		INITIAL_SOCKET_COUNT = 10,
		RECREATE_SOCKET_COUNT = 10
	};

private:
	void CreateSessionSocket(int _count);

public:
	size_t GetSessionSocketCount() { return m_sessionSocketPool.size(); }

	SessionSocket* GetSessionSocket();
	void RetrieveSessionSocket(SessionSocket* _sessionSocket);
};

