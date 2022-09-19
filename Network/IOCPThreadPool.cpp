#include "pch.h"
#include "IOCPThreadPool.h"

IOCPThreadPool::IOCPThreadPool(UINT _threadCount)
{
	for (int i = 0; i < _threadCount; ++i)
	{
		m_theadArray.push_back(std::thread(&IOCPThreadPool::ThreadWork, this));
	}

	m_IOCPHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, _threadCount);
}

IOCPThreadPool::~IOCPThreadPool()
{
	while (0 < m_jobCount)
	{
		Sleep(1);
	}

	int threadCount = m_theadArray.size();
	for (int i = 0; i < threadCount; ++i)
	{
		Stop();
	}

	for (int i = 0; i < m_theadArray.size(); ++i)
	{
		m_theadArray[i].join();
	}

	if (NULL != m_IOCPHandle)
	{
		CloseHandle(m_IOCPHandle);
	}
}

void IOCPThreadPool::ThreadWork()
{
	while (true)
	{
		DWORD			transferredBytes;
		ULONG_PTR		completionKey;
		LPOVERLAPPED	overlapped;
		int result = GetQueuedCompletionStatus(m_IOCPHandle, &transferredBytes, &completionKey, &overlapped, INFINITE);
		if (result == 0)
		{
			// 에러 처리
			int error = GetLastError();
			return;
		}

		// 종료 처리
		if (transferredBytes == 0)
			break;
		

		std::function<void()>* pWork = reinterpret_cast<std::function<void()>*>(completionKey);
		(*pWork)();

		--m_jobCount;
		delete pWork;
	}
}

void IOCPThreadPool::Stop()
{
	PostQueuedCompletionStatus(m_IOCPHandle, 0, NULL, NULL);
}

void IOCPThreadPool::AddWork(std::function<void()> _work)
{
	// AddWork가 많아지면 메모리 부족
	std::function<void()>* pWork = new std::function<void()>(_work);
	PostQueuedCompletionStatus(m_IOCPHandle, 1, reinterpret_cast<ULONG_PTR>(pWork), NULL);
	++m_jobCount;
}
