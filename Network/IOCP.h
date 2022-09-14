#pragma once
#include "Singleton.h"
#include "ThreadPool.h"	

class IOCP : public Singleton<IOCP>
{
	friend class Singleton;

private:
	IOCP();
	~IOCP();

private:
	int							m_logicThreadCount;
	HANDLE						m_completionPort;
	std::vector<std::thread>	m_IOThreadPool;

	ThreadPool					m_logicThreadPool;

	enum
	{
		THREAD_EXIT_CODE = -1
	};

private:
	void IOWorkThread();

public:
	void RegisterSocket(SOCKET _socket, IOCompletionCallback* _pIoCompletionCallback);
};
