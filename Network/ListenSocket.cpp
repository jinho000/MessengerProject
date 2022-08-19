#include "pch.h"
#include "ListenSocket.h"
#include "IOCP.h"
#include "SessionManager.h"
#include "SessionSocket.h"

ListenSocket::ListenSocket(int _serverPort, std::string _serverIP)
	: Socket(_serverPort, _serverIP, IPPROTO::IPPROTO_TCP)
{
	// ���Ͽ� �ּ�ü�� ���ε�
	if (SOCKET_ERROR == bind(m_socket, (const sockaddr*)&m_address, sizeof(m_address)))
	{
		CloseSocket();
		ServerHelper::PrintLastError("socket bind error");
		return;
	}

	// �������� �ɼ� ����
	BOOL on = TRUE;
	int result = setsockopt(m_socket, SOL_SOCKET, SO_CONDITIONAL_ACCEPT, reinterpret_cast<const char*>(&on), sizeof(on));
	if (SOCKET_ERROR == result)
	{
		CloseSocket();
		ServerHelper::PrintLastError("listen socket option error");
		return;
	}

}

ListenSocket::~ListenSocket()
{
}

//void ListenSocket::ListenCompleteCallback(DWORD _transferredBytes, LPOVERLAPPED _IOData)
//{
//	// ����
//	// ���Ǽ��Ͽ� IOCompletion Callback �Լ� �����
//	
//	// ����
//	// Completion Port�� AcceptEx�� ������ ����ص�ä �����ϸ� 995 ����
//	// CompletionData �����Ҵ����� ���� �޸� ����
//
//
//	IOCompletionData* completionData = reinterpret_cast<IOCompletionData*> (_IOData);
//	
//	// ���Ǽ��Ͽ� ������ ����
//	SessionSocket* pSessionSocket = completionData->pSessionSocket;
//	if (pSessionSocket->IsUsed() == false)
//	{
//		// ������ ����Ǹ� IOCP�� ��ϵ� ���ϵ� ����Ǵ��� Ȯ��
//		// �ѹ��� ��ϵ��� ���� ���ϸ� ���
//		//IOCP::GetInst()->RegisterSocket(pSessionSocket->GetSocket(), );
//	}
//
//	// �ּҰ� ����
//	SOCKADDR_IN* pLocalAddr = nullptr;
//	SOCKADDR_IN* pClientAddress = nullptr;
//
//	int localLen = 0;
//	int RemoteLen = 0;
//	GetAcceptExSockaddrs(completionData->buffer,
//		0,
//		sizeof(sockaddr_in) + 16,
//		sizeof(sockaddr_in) + 16,
//		(LPSOCKADDR*)&pLocalAddr,
//		&localLen,
//		(LPSOCKADDR*)&pClientAddress,
//		&RemoteLen);
//	
//	pSessionSocket->SetClientAddress(*pClientAddress);
//
//	std::cout << pSessionSocket->GetClientIP() << std::endl;
//	std::cout << pSessionSocket->GetClientPort() << std::endl;
//
//	// ���� �Ŵ����� ���� ����
//	SessionManager::GetInst()->AddSessionSocket(pSessionSocket);
//
//	// ����� �񵿱� �Ϸ� ������ ����
//	delete completionData;
//
//	// �ٽ� accecpt ��û
//	RequestAsyncAccept();
//}
//

