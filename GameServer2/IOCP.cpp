#include "pch.h"
#include "IOCP.h"
#include "TCPListener.h"
#include "SessionManager.h"

IOCP* IOCP::pInstance = nullptr;

IOCP::IOCP()
	: m_threadCount(0)
	, m_completionPort(NULL)
{
	// CPU개수 * 2 - 1만큼 스레드 만들기
	// 1개는 메인 스레드
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);

	m_threadCount = sysinfo.dwNumberOfProcessors * 2 - 1;
	m_completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, m_threadCount);

	if (m_completionPort == NULL)
	{
		ServerHelper::PrintLastError("Create Completion Port Error");
		return;
	}

	// 지정된 개수만큼 스레드 만들기
	for (int i = 0; i < m_threadCount; ++i)
	{
		std::thread worker(std::bind(&IOCP::WorkThread, this));
		m_workerThreadArry.push_back(std::move(worker));
	}
}

IOCP::~IOCP()
{
	// IOCP에 연결되어있는 소켓이 모두 close된 후 스레드를 종료해야함
	// 연결된 소켓들이 모두 종료될때까지 컨텍스트 스위칭
	while (TCPListener::GetInst() != nullptr)
	{
		Sleep(1);
	}

	while (SessionManager::GetInst() != nullptr)
	{
		Sleep(1);
	}

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
			// Completion Port에 AcceptEx로 소켓을 등록해둔채 종료하면 995 에러
			ServerHelper::PrintLastError("GetQueuedCompletionStauts Error");
			return;
		}

		// 스레드 종료체크
		if (transferredBytes == THREAD_EXIT_CODE && completionKey == THREAD_EXIT_CODE)
			break;

		// 비동기 입출력 결과 처리
		IOCompletionCallback* pIOCallback = reinterpret_cast<IOCompletionCallback*>(completionKey);
		assert(pIOCallback != nullptr);

		(*pIOCallback)(transferredBytes, overlapped);

	}
}


void IOCP::RegisterSocket(SOCKET _socket, IOCompletionCallback* _pIoCompletionCallback)
{
	if (NULL == CreateIoCompletionPort(reinterpret_cast<HANDLE>(_socket), m_completionPort, reinterpret_cast<ULONG_PTR>(_pIoCompletionCallback), 0))
	{
		ServerHelper::PrintLastError("Register Socket Error");
		return;
	}
}
