#include "pch.h"
#include "IOCP.h"
#include "TCPListener.h"
#include "SessionManager.h"
#include "ConfigManager.h"

IOCP::IOCP()
	: m_completionPort(NULL)
	, m_coreCountHalf(std::thread::hardware_concurrency() / 2)
	, m_logicThreadPool(m_coreCountHalf)
{
	m_completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, m_coreCountHalf);

	if (m_completionPort == NULL)
	{
		ServerHelper::PrintLastError("Create Completion Port Error");
		return;
	}

	// 지정된 개수만큼 스레드 만들기
	for (int i = 0; i < m_coreCountHalf; ++i)
	{
		m_IOThreadArray.push_back(std::thread(&IOCP::IOWorkThread, this));
	}
}

IOCP::~IOCP()
{
	// 비동기 완료작업 종료 요청
	for (size_t i = 0; i < m_IOThreadArray.size(); i++)
	{
		// 스레드를 종료할때만 PostQueued 작업 요청
		PostQueuedCompletionStatus(m_completionPort, THREAD_EXIT_CODE, THREAD_EXIT_CODE, nullptr);
	}

	for (size_t i = 0; i < m_IOThreadArray.size(); i++)
	{
		m_IOThreadArray[i].join();
	}
}

void IOCP::IOWorkThread()
{
	while (true)
	{
		DWORD			transferredBytes;
		ULONG_PTR		completionKey;
		LPOVERLAPPED	overlapped;
		BOOL result = GetQueuedCompletionStatus(m_completionPort, &transferredBytes, &completionKey, &overlapped, INFINITE);

		// 클라이언트가 강제종료시 result = false, transferredBytes = 0
		// 클라이언트가 정상종료시 result = true,  transferredBytes = 0
		if (result == FALSE && transferredBytes != 0)
		{
			ServerHelper::PrintLastError("GetQueuedCompletionStauts Error");
			return;
		}


		// 스레드 종료체크
		if (transferredBytes == THREAD_EXIT_CODE && completionKey == THREAD_EXIT_CODE)
			break;

		// 비동기 입출력 결과 처리
		assert(completionKey != 0);
		assert(overlapped != nullptr);

		//IOCompletionCallback* pIOCallback = reinterpret_cast<IOCompletionCallback*>(completionKey);
		//IOCompletionData* ioCompletionData = reinterpret_cast<IOCompletionData*>(overlapped);
		//(*pIOCallback)(transferredBytes, ioCompletionData);

		// 생산자 소비자 모델
		// IOCP에서 입출력 데이터를 받아 m_logicThreadPool에 전달 하여 처리
		// 입출력을 받아오는 생산자 스레드와 입출력을 처리하는 소비자 스레드를 만들어 따로 처리
		m_logicThreadPool.AddWork([completionKey, transferredBytes, overlapped]() {
			IOCompletionCallback* pIOCallback = reinterpret_cast<IOCompletionCallback*>(completionKey);
			IOCompletionData* ioCompletionData = reinterpret_cast<IOCompletionData*>(overlapped);
			(*pIOCallback)(transferredBytes, ioCompletionData);
		});

	}
}


void IOCP::RegisterSocket(SOCKET _socket, IOCompletionCallback* _pIoCompletionCallback)
{
	if (NULL == CreateIoCompletionPort(
		reinterpret_cast<HANDLE>(_socket)
		, m_completionPort
		, reinterpret_cast<ULONG_PTR>(_pIoCompletionCallback)
		, 0))
	{
		ServerHelper::PrintLastError("Register Socket Error");
		return;
	}
}
