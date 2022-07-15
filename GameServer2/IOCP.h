#pragma once


class IOCP
{
private: // member var
	static IOCP* pInstance;

private:
	IOCP();
	~IOCP();

	IOCP(const IOCP& _other) = delete;
	IOCP(IOCP&& _other) = delete;
	IOCP& operator=(const IOCP& _other) = delete;
	IOCP& operator=(const IOCP&& _other) = delete;

public:
	static void CreateInstance(int _threadCount);
	static void Destroy();
	static IOCP* GetInst();

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

