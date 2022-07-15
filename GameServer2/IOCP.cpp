#include "pch.h"
#include "IOCP.h"

IOCP* IOCP::pInstance = nullptr;

IOCP::IOCP()
	: m_threadCount(0)
	, m_completionPort(NULL)
{
}

IOCP::~IOCP()
{
	// 스레드 종료
	for (size_t i = 0; i < m_workerThreadArry.size(); i++)
	{
		// 스레드를 종료할때만 PostQueued 작업 요청
		PostQueuedCompletionStatus(m_completionPort, THREAD_EXIT_CODE, THREAD_EXIT_CODE, nullptr);
	}

	for (size_t i = 0; i < m_workerThreadArry.size(); i++)
	{
		m_workerThreadArry[i].join();
	}
}

void IOCP::WorkThread()
{
	while (true)
	{
		DWORD			transferredBytes;
		ULONG_PTR		completionKey;
		LPOVERLAPPED	overlapped;
		BOOL result = GetQueuedCompletionStatus(m_completionPort, &transferredBytes, &completionKey, &overlapped, INFINITE);

		// 클라이언트가 강제종료시 result = false, transferredBytes = 0
		// 클라이언트가 정상종료시 result = true,  transferredBytes = 0
		if (result == FALSE)
		{
			ServerHelper::PrintLastError("GetQueuedCompletionStauts Error");
			return;
		}

		// 스레드 종료체크
		if (transferredBytes == THREAD_EXIT_CODE && completionKey == THREAD_EXIT_CODE)
			break;

		// 비동기 입출력 결과 처리
		IOCompletionCallback* pIOCallback = reinterpret_cast<IOCompletionCallback*>(completionKey);

		(*pIOCallback)(transferredBytes, overlapped);

	}
}


void IOCP::CreateInstance(int _threadCount)
{
	if (pInstance != nullptr)
		return;

	pInstance = new IOCP;
	pInstance->m_threadCount = _threadCount;
	pInstance->m_completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, pInstance->m_threadCount);

	if (pInstance->m_completionPort == NULL)
	{
		ServerHelper::PrintLastError("Create Completion Port Error");
		return;
	}

	// CPU개수 * 2 - 1만큼 스레드 만들기
	// 1개는 메인 스레드
	//SYSTEM_INFO sysinfo;
	//GetSystemInfo(&sysinfo);

	// 지정된 개수만큼 스레드 만들기
	for (int i = 0; i < pInstance->m_threadCount; ++i)
	{
		std::thread worker(std::bind(&IOCP::WorkThread, pInstance));
		pInstance->m_workerThreadArry.push_back(std::move(worker));
	}
}

void IOCP::Destroy()
{
	if (pInstance != nullptr)
	{
		delete pInstance;
		pInstance = nullptr;
	}
}

IOCP* IOCP::GetInst()
{
	return pInstance;
}

void IOCP::RegisterSocket(SOCKET _socket, IOCompletionCallback* _pIoCompletionCallback)
{
	if (NULL == CreateIoCompletionPort(reinterpret_cast<HANDLE>(_socket), m_completionPort, reinterpret_cast<ULONG_PTR>(_pIoCompletionCallback), 0))
	{
		ServerHelper::PrintLastError("Register Socket Error");
		return;
	}
}
