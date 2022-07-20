#include "pch.h"
#include "TCPSession.h"
#include "TCPListener.h"
#include "IOCP.h"
#include "TCPSessionPool.h"
#include "SessionManager.h"

TCPSession::TCPSession()
	: m_sessionSocket()
	, m_IOCompletionAccept(*this, IOTYPE::ACCEPT)
	, m_IOCompletionDisconnect(*this, IOTYPE::DISCONNECT)
	, m_IOCompletionRecv(*this, IOTYPE::RECV)
	, m_IOCompletionSend(*this, IOTYPE::SEND)
	, m_IOCompletionCallback(std::bind(&TCPSession::IOCompletionCallback, this, std::placeholders::_1, std::placeholders::_2))
{
}

TCPSession::~TCPSession()
{
}

void TCPSession::IOCompletionCallback(DWORD _transferredBytes, IOCompletionData* _IOData)
{
	switch (_IOData->IOType)
	{
	case IOTYPE::RECV:
	{
		// client 접속 종료 처리
		if (_transferredBytes == 0)
		{
			// 소켓 종료
			BOOL bResult = TransmitFile(m_sessionSocket.GetSocket(), 0, 0, 0
				, &m_IOCompletionDisconnect.overlapped
				, 0
				, TF_DISCONNECT | TF_REUSE_SOCKET
			);

			if (FALSE == bResult && ERROR_IO_PENDING != WSAGetLastError())
			{
				ServerHelper::PrintLastError("TransmitFile error");
			}

			break;
		}

		// echo
		m_IOCompletionRecv.buffer[_transferredBytes] = '\0';
		std::cout << m_IOCompletionRecv.buffer << std::endl;

		std::string buffer(m_IOCompletionRecv.buffer);
		std::vector<uint8_t> data;
		data.resize(buffer.size() + 1, 0);
		std::copy(buffer.begin(), buffer.end(), data.begin());
		RequestSend(data);

		// Recv 처리
		// PacketHandler::GetInst()->Dispatch(_IOData->buffer);
		// 
		// HandleFunc df
		// df(); 
		// 
		// 

		RequestRecv();
		break;
	}
	case IOTYPE::SEND:
	{
		break;
	}
	case IOTYPE::DISCONNECT:
	{
		// 세션 매니저에서 꺼내기
		SessionManager::GetInst()->PopTCPSession(this);

		// 세션 풀에 넣기 
		TCPSessionPool::GetInst()->RetrieveTCPSession(this);
		break;
	}
	default:
		break;
	}

}


bool TCPSession::IsRecycleSession()
{
	// 세션소켓이 IOCP에 등록 됐는지 여부로 재사용된 세션인지 판단
    return m_sessionSocket.IsRegistIOCP();
}

void TCPSession::RequestAsyncAccept()
{
    DWORD dwByte = 0;
    BOOL result = AcceptEx(TCPListener::GetInst()->GetListenSocket()
        , m_sessionSocket.GetSocket()
        , m_IOCompletionAccept.buffer
		, 0
		, sizeof(sockaddr_in) + 16
		, sizeof(sockaddr_in) + 16
		, &dwByte
		, &m_IOCompletionAccept.overlapped);

	if (FALSE == result)
	{
		if (ERROR_IO_PENDING != WSAGetLastError())
		{
			ServerHelper::PrintLastError("AcceptEx Error");
			return;
		}
	}

}

void TCPSession::SetClientAddress()
{
	SOCKADDR_IN* pLocalAddr = nullptr;
	SOCKADDR_IN* pClientAddress = nullptr;

	int localLen = 0;
	int RemoteLen = 0;
	GetAcceptExSockaddrs(m_IOCompletionRecv.buffer,
		0,
		sizeof(sockaddr_in) + 16,
		sizeof(sockaddr_in) + 16,
		(LPSOCKADDR*)&pLocalAddr,
		&localLen,
		(LPSOCKADDR*)&pClientAddress,
		&RemoteLen);

	m_sessionSocket.SetClientAddress(*pClientAddress);
}

void TCPSession::RegistIOCP()
{
	if (m_sessionSocket.IsRegistIOCP() == false)
	{
		// 한번도 등록되지 않은 소켓만 등록
		m_sessionSocket.SetRegistIOCP();
		IOCP::GetInst()->RegisterSocket(m_sessionSocket.GetSocket(), &m_IOCompletionCallback);
	}
}

void TCPSession::RequestRecv()
{
	DWORD recvByte = 0;
	DWORD dwFlags = 0;

	// 리시브 요청을 할 때마다 IOCompletionData 정리
	m_IOCompletionRecv.Clear();

	if (SOCKET_ERROR == WSARecv(
		m_sessionSocket.GetSocket()
		, &m_IOCompletionRecv.wsabuf
		, 1
		, &recvByte
		, &dwFlags
		, &m_IOCompletionRecv.overlapped
		, nullptr))
	{
		int Error = WSAGetLastError();
		if (WSA_IO_PENDING != Error)
		{
			ServerHelper::PrintLastError("WSARecv Error");
			return;
		}
	}
}

void TCPSession::RequestSend(const std::vector<uint8_t>& _buffer)
{
	if (_buffer.empty())
	{
		return;
	}

	// Send 요청시 마다 클리어
	m_IOCompletionSend.Clear();
	m_IOCompletionSend.SetBuffer(_buffer);

	DWORD byteSize = 0;
	DWORD flag = 0;
	int result = WSASend(m_sessionSocket.GetSocket()
		, &m_IOCompletionSend.wsabuf
		, 1
		, &byteSize
		, flag
		, &m_IOCompletionSend.overlapped
		, nullptr
	);

	if (SOCKET_ERROR == result)
	{
		if (WSA_IO_PENDING != WSAGetLastError())
		{
			ServerHelper::PrintLastError("WSASend Error");
			return;
		}
	}
}




