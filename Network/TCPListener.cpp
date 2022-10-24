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
	// ���޵� Overlapped ����ü�� IOCompletionData�� ĳ���� �� TCPSession ������
	// ���� Ŭ���̾�Ʈ�� ���� ���� ó��
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

	// ����Ǯ���� ������
	TCPSessionPool::GetInst()->PopTCPSession(&rfTcpSession);

	// ���� �Ŵ����� ���� ����
	SessionManager::GetInst()->AddTCPSession(&rfTcpSession);
}

void TCPListener::StartListen()
{
	// backlog�� �ִ밪���� ����
	listen(m_listenSocket.GetSocket(), SOMAXCONN);

	// IOCP�� �������� ���
	IOCP::GetInst()->RegisterSocket(m_listenSocket.GetSocket(), &m_listenCompleteCallback);
}
