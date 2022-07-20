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
	// accpetEx 중인데 종료한경우
	// IOCP work스레드가 종료되어 accept상태에서 소켓이 빠져나와야한다
	// accept중이었던 TCP세션을 여기서 종료
	auto iter = m_acceptingTCPSession.begin();
	while (iter != m_acceptingTCPSession.end())
	{
		// delete를 호출하는 스레드는 하나뿐이므로 락을걸지 않는다
		delete iter->second;
		iter->second = nullptr;

		++iter;
	}
}

void TCPListener::ListenCompleteCallback(DWORD _transferredBytes, IOCompletionData* _IOData)
{	
	// 전달된 Overlapped 구조체를 IOCompletionData로 캐스팅 후 TCPSession 얻어오기
	TCPSession& rfTcpSession = _IOData->tcpSession;
	rfTcpSession.SetClientAddress();
	rfTcpSession.RegistIOCP();
	rfTcpSession.RequestRecv();

	// 관리하고있는 세션맵에서 꺼내기
	m_acceptingTCPSession.erase(&rfTcpSession);

	// 세션 매니저에 소켓 전달
	SessionManager::GetInst()->AddTCPSession(&rfTcpSession);

	// SessionPool에서 TCPSession을 받아 accept 요청
	TCPSession* pTCPSession = TCPSessionPool::GetInst()->GetTCPSession();
	pTCPSession->RequestAsyncAccept();
	m_acceptingTCPSession.insert({ pTCPSession, pTCPSession });
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
		m_acceptingTCPSession.insert({ pTCPSession, pTCPSession });
	}

}
