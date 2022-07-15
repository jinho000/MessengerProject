#include "pch.h"
#include "ListenSocket.h"
#include "IOCP.h"
#include "SessionManager.h"
#include "SessionSocketPool.h"
#include "SessionSocket.h"

ListenSocket::ListenSocket(int _serverPort, std::string _serverIP)
	: Socket(_serverPort, _serverIP, IPPROTO::IPPROTO_TCP)
{
	// 리슨소켓 옵션 설정
	BOOL on = TRUE;
	int result = setsockopt(m_socket, SOL_SOCKET, SO_CONDITIONAL_ACCEPT, reinterpret_cast<const char*>(&on), sizeof(on));
	if (SOCKET_ERROR == result)
	{
		CloseSocket();
		ServerHelper::PrintLastError("listen socket option error");
		return;
	}

	m_listenCompleteCallback = std::bind(&ListenSocket::ListenCompleteCallback, this, std::placeholders::_1, std::placeholders::_2);
}

void ListenSocket::ListenCompleteCallback(DWORD _transferredBytes, LPOVERLAPPED _IOData)
{
	// 할일
	// 세션소켓에 IOCompletion Callback 함수 만들기
	
	// 문제
	// Completion Port에 AcceptEx로 소켓을 등록해둔채 종료하면 995 에러
	// completionData 동적할당으로 인한 메모리 누수


	IOCompletionData* completionData = reinterpret_cast<IOCompletionData*> (_IOData);
	
	// 세션소켓에 데이터 세팅
	SessionSocket* pSessionSocket = completionData->pSessionSocket;
	if (pSessionSocket->IsUsed() == false)
	{
		// 소켓이 종료되면 IOCP에 등록된 소켓도 종료되는지 확인
		// 한번도 등록되지 않은 소켓만 등록
		//IOCP::GetInst()->RegisterSocket(pSessionSocket->GetSocket(), );
	}

	// 주소값 세팅
	SOCKADDR_IN* pLocalAddr = nullptr;
	SOCKADDR_IN* pClientAddress = nullptr;

	int localLen = 0;
	int RemoteLen = 0;
	GetAcceptExSockaddrs(completionData->buffer,
		0,
		sizeof(sockaddr_in) + 16,
		sizeof(sockaddr_in) + 16,
		(LPSOCKADDR*)&pLocalAddr,
		&localLen,
		(LPSOCKADDR*)&pClientAddress,
		&RemoteLen);
	
	pSessionSocket->SetClientAddress(*pClientAddress);

	std::cout << pSessionSocket->GetClientIP() << std::endl;
	std::cout << pSessionSocket->GetClientPort() << std::endl;

	// 세션 매니저에 소켓 전달
	SessionManager::GetInst()->AddSessionSocket(pSessionSocket);

	// 사용한 비동기 완료 데이터 삭제
	delete completionData;

	// 다시 accecpt 요청
	RequestAsyncAccept();
}

void ListenSocket::RequestAsyncAccept()
{
	SessionSocket* pSessionSocket = SessionSocketPool::GetInst()->GetSessionSocket();
	IOCompletionData* completionData = new IOCompletionData(pSessionSocket);
	
	DWORD dwByte = 0;
	BOOL result = AcceptEx(m_socket
		, pSessionSocket->GetSocket()
		, completionData->buffer
		, 0
		, sizeof(sockaddr_in) + 16
		, sizeof(sockaddr_in) + 16
		, &dwByte
		, &completionData->overlapped);
}

void ListenSocket::StartListen()
{
	// IOCP에 리슨소켓 등록
	IOCP::GetInst()->RegisterSocket(m_socket, &m_listenCompleteCallback);

	// backlog는 최대값으로 설정
	listen(m_socket, SOMAXCONN);

	size_t initialSocketCount = SessionSocketPool::GetInst()->GetSessionSocketCount();
	for (size_t i = 0; i < initialSocketCount; i++)
	{
		RequestAsyncAccept();
	}
}

