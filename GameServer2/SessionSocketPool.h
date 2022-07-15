#pragma once

class SessionSocket;
class SessionSocketPool
{
private: // member var
	static SessionSocketPool* pInstance;

private: // default
	SessionSocketPool(int _initialSocketCount);
	~SessionSocketPool();

	SessionSocketPool(const SessionSocketPool& _other) = delete;
	SessionSocketPool(SessionSocketPool&& _other) = delete;
	SessionSocketPool& operator=(const SessionSocketPool& _other) = delete;
	SessionSocketPool& operator=(const SessionSocketPool&& _other) = delete;

public: // member Func
	static void CreateInstance(int _initialSocketCount);
	static void Destroy();
	static SessionSocketPool* GetInst();

private:
	std::queue<SessionSocket*>	m_sessionSocketPool;
	std::mutex					m_lock;

	enum
	{
		RECREATE_SOCKET_COUNT = 10
	};

private:
	void CreateSessionSocket(int _count);

public:
	size_t GetSessionSocketCount() { return m_sessionSocketPool.size(); }

	SessionSocket* GetSessionSocket();
	void RetrieveSessionSocket(SessionSocket* _sessionSocket);
};

