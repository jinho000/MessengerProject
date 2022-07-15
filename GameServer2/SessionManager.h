#pragma once

class SessionSocket;
class SessionManager
{
private: // member var
	static SessionManager* pInstance;

private: // default
	SessionManager();
	~SessionManager();

	SessionManager(const SessionManager& _other) = delete;
	SessionManager(SessionManager&& _other) = delete;
	SessionManager& operator=(const SessionManager& _other) = delete;
	SessionManager& operator=(const SessionManager&& _other) = delete;

public: // member Func
	static void CreateInstance();
	static void Destroy();
	static SessionManager* GetInst();

private:
	std::list<SessionSocket*>	m_socketList;
	std::mutex					m_lock;

public:
	void AddSessionSocket(SessionSocket* _pSessionSocket);

};

