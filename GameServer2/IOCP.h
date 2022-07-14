#pragma once


class IOCP
{
private: // member var
	static IOCP* pInstance;
	enum
	{
		THREAD_EXIT_CODE = -1
	};

private:
	int							m_threadCount;
	HANDLE						m_completionPort;
	std::vector<std::thread>	m_workerThreadArry;

private:
	IOCP();
	~IOCP();

	IOCP(const IOCP& _other) = delete;
	IOCP(IOCP&& _other) = delete;
	IOCP& operator=(const IOCP& _other) = delete;
	IOCP& operator=(const IOCP&& _other) = delete;

private:
	void WorkThread();

public:
	static void CreateInstance(int _threadCount);
	static void Destroy();
	static IOCP* GetInst();

public:
	void RegisterSocket(SOCKET _socket, IOCompletionCallback* _pIoCompletionCallback);

};

