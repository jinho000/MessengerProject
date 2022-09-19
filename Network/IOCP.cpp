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

	// ������ ������ŭ ������ �����
	for (int i = 0; i < m_coreCountHalf; ++i)
	{
		m_IOThreadArray.push_back(std::thread(&IOCP::IOWorkThread, this));
	}
}

IOCP::~IOCP()
{
	// �񵿱� �Ϸ��۾� ���� ��û
	for (size_t i = 0; i < m_IOThreadArray.size(); i++)
	{
		// �����带 �����Ҷ��� PostQueued �۾� ��û
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

		// Ŭ���̾�Ʈ�� ��������� result = false, transferredBytes = 0
		// Ŭ���̾�Ʈ�� ��������� result = true,  transferredBytes = 0
		if (result == FALSE && transferredBytes != 0)
		{
			ServerHelper::PrintLastError("GetQueuedCompletionStauts Error");
			return;
		}


		// ������ ����üũ
		if (transferredBytes == THREAD_EXIT_CODE && completionKey == THREAD_EXIT_CODE)
			break;

		// �񵿱� ����� ��� ó��
		assert(completionKey != 0);
		assert(overlapped != nullptr);

		//IOCompletionCallback* pIOCallback = reinterpret_cast<IOCompletionCallback*>(completionKey);
		//IOCompletionData* ioCompletionData = reinterpret_cast<IOCompletionData*>(overlapped);
		//(*pIOCallback)(transferredBytes, ioCompletionData);

		// ������ �Һ��� ��
		// IOCP���� ����� �����͸� �޾� m_logicThreadPool�� ���� �Ͽ� ó��
		// ������� �޾ƿ��� ������ ������� ������� ó���ϴ� �Һ��� �����带 ����� ���� ó��
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
