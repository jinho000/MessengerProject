#include "pch.h"
#include "TCPListener.h"
#include "IOCP.h"
#include "ConfigManager.h"
#include "SessionManager.h"
#include "TCPSessionPool.h"
#include "TCPSession.h"
#include "Logger.h"

TCPListener::TCPListener()
	: m_listenSocket(ConfigManager::GetInst()->GetServerPort(), ConfigManager::GetInst()->GetServerIP())
{
	m_listenCompleteCallback = std::bind(&TCPListener::ListenCompleteCallback, this, std::placeholders::_1, std::placeholders::_2);
}

TCPListener::~TCPListener()
{
}

void TCPListener::ListenCompleteCallback(DWORD _transferredBytes, IOCompletionData* _IOData)
{	
	// 전달된 Overlapped 구조체를 IOCompletionData로 캐스팅 후 TCPSession 얻어오기
	// 들어온 클라이언트에 대한 세팅 처리
	TCPSession& rfTcpSession = _IOData->tcpSession;
	rfTcpSession.SetClientAddress();
	rfTcpSession.RegistIOCP();
	rfTcpSession.RequestRecv();

	const SessionSocket& sessionSocket = rfTcpSession.GetSessionSocket();
	std::string log;
	log += "Connect Client\n";
	log += "Client IP: " + sessionSocket.GetClientIP() + "\n";
	log += "Client Port: " + std::to_string(sessionSocket.GetClientPort()) + "\n";
	Logger::GetInst()->Log(log);

	// 세션풀에서 꺼내기
	TCPSessionPool::GetInst()->PopTCPSession(&rfTcpSession);

	// 세션 매니저에 소켓 전달
	SessionManager::GetInst()->AddTCPSession(&rfTcpSession);
}

void TCPListener::StartListen()
{
	// backlog는 최대값으로 설정
	listen(m_listenSocket.GetSocket(), SOMAXCONN);

	// IOCP에 리슨소켓 등록
	IOCP::GetInst()->RegisterSocket(m_listenSocket.GetSocket(), &m_listenCompleteCallback);
}
