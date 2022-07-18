#include "pch.h"
#include "TCPListener.h"
#include "IOCP.h"
#include "ConfigManager.h"
#include "SessionManager.h"
#include "TCPSessionPool.h"
#include "TCPSession.h"

TCPListener::TCPListener()
	: m_listenSocket(ConfigManager::GetInst()->GetServerPort(), ConfigManager::GetInst()->GetServerIP())
{
	m_listenCompleteCallback = std::bind(&TCPListener::ListenCompleteCallback, this, std::placeholders::_1, std::placeholders::_2);
}

TCPListener::~TCPListener()
{
	// accpetEx 중인데 종료한경우?
	while (m_acceptingTCPSession.empty() == false)
	{
		// delete를 호출하는 스레드는 하나뿐이므로 락을걸지 않는다
		TCPSession* pSessionSocket = m_acceptingTCPSession.front(); m_acceptingTCPSession.pop_front();
		delete pSessionSocket;
		pSessionSocket = nullptr;
	}
}

void TCPListener::ListenCompleteCallback(DWORD _transferredBytes, LPOVERLAPPED _IOData)
{	
	// 문제
	// Completion Port에 AcceptEx로 소켓을 등록해둔채 종료하면 995 에러
	
	// 전달된 Overlapped 구조체를 IOCompletionData로 캐스팅 후 TCPSession 얻어오기
	TCPSession& rfTcpSession = reinterpret_cast<IOCompletionData*> (_IOData)->tcpSession;
	rfTcpSession.SetClientAddress();
	rfTcpSession.RegistIOCP();
	rfTcpSession.RequestRecv();

	// 리스너에서 꺼내기 
	auto iter = m_acceptingTCPSession.begin();
	while (iter != m_acceptingTCPSession.end())
	{
		if (*iter == &rfTcpSession)
		{
			m_acceptingTCPSession.erase(iter);
			break;
		}

		++iter;
	}

	// 세션 매니저에 소켓 전달
	SessionManager::GetInst()->AddTCPSession(&rfTcpSession);


	// SessionPool에서 TCPSession을 받아 accept 요청
	TCPSession* pTCPSession = TCPSessionPool::GetInst()->GetTCPSession();
	pTCPSession->RequestAsyncAccept();
}

void TCPListener::StartListen()
{
	// backlog는 최대값으로 설정
	listen(m_listenSocket.GetSocket(), SOMAXCONN);

	// IOCP에 리슨소켓 등록
	IOCP::GetInst()->RegisterSocket(m_listenSocket.GetSocket(), &m_listenCompleteCallback);

	// TCPSession을 가져와 acceptEx 호출
	// accpetEx를 호출한 TCPSession은 TCPListener에서 관리
	for (size_t i = 0; i < InitialAcceptSocketCount; i++)
	{
		TCPSession* pTCPSession = TCPSessionPool::GetInst()->GetTCPSession();
		pTCPSession->RequestAsyncAccept();
		m_acceptingTCPSession.push_back(pTCPSession);
	}

}
