#pragma once
#include "Singleton.h"
#include "IOCPThreadPool.h"

class IOCP : public Singleton<IOCP>
{
	friend class Singleton;

private:
	IOCP();
	~IOCP();

private:
	HANDLE						m_completionPort;
	std::vector<std::thread>	m_IOThreadArray;

	int							m_coreCountHalf;
	IOCPThreadPool				m_logicThreadPool;

	enum
	{
		THREAD_EXIT_CODE = -1
	};

private:
	void IOWorkThread();

public:
	void RegisterSocket(SOCKET _socket, IOCompletionCallback* _pIoCompletionCallback);
};
