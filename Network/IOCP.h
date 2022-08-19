#pragma once
#include "Singleton.h"

class IOCP : public Singleton<IOCP>
{
	friend class Singleton;

private:
	IOCP();
	~IOCP();

private:
	int							m_threadCount;
	HANDLE						m_completionPort;
	std::vector<std::thread>	m_workerThreadArry;
	
	enum
	{
		THREAD_EXIT_CODE = -1
	};

private:
	void WorkThread();

public:
	void RegisterSocket(SOCKET _socket, IOCompletionCallback* _pIoCompletionCallback);
};
