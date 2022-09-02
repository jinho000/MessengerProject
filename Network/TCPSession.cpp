#include "pch.h"
#include "TCPSession.h"
#include "TCPListener.h"
#include "IOCP.h"
#include "TCPSessionPool.h"
#include "SessionManager.h"
#include "PacketHandler.h"

#include <PacketLibrary/PacketBase.h>
#include <PacketLibrary/Serializer.h>
#include <PacketLibrary/PacketHelper.h>

TCPSession::TCPSession()
	: m_sessionSocket()
	, m_IOCompletionAccept(*this, IOTYPE::ACCEPT)
	, m_IOCompletionDisconnect(*this, IOTYPE::DISCONNECT)
	, m_IOCompletionRecv(*this, IOTYPE::RECV)
	, m_IOCompletionSend(*this, IOTYPE::SEND)
	, m_IOCompletionCallback(std::bind(&TCPSession::IOCompletionCallback, this, std::placeholders::_1, std::placeholders::_2))
	, m_packetSize(-1)
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

		// TCP의 데이터는 경계가 존재하지 않음
		// 데이터가 패킷의 사이즈만큼 왔는지 확인하기
		// 들어온 데이터 개수만큼 리시브 버퍼 뒤에 저장
		m_recvBuffer.insert(m_recvBuffer.end(), m_IOCompletionRecv.buffer, m_IOCompletionRecv.buffer + _transferredBytes);

		// 리시브버퍼에 패킷헤더의 데이터가 들어왔는지 확인
		if (PacketBase::SIZEOF_PACKET_HEADER < m_recvBuffer.size())
		{
			m_packetSize = *reinterpret_cast<int*>(m_recvBuffer.data() + sizeof(PACKET_TYPE));
		}

		// 리시브버퍼에 전체 패킷데이터가 들어왔는지 확인
		if (m_packetSize <= m_recvBuffer.size())
		{
			// m_recvBuffer에서 패킷크기만큼 데이터를 가져와 버퍼에 채움
			std::vector<uint8_t> buffer;
			buffer.assign(m_recvBuffer.begin(), m_recvBuffer.begin() + m_packetSize);
	
			// 들어온 패킷 처리
			std::unique_ptr<PacketBase> pPacket = PacketHelper::ConvertToPacket(buffer);
			assert(pPacket != nullptr);
			PacketHandler::GetInst()->DispatchPacket(this, std::move(pPacket));

			// 사용한 데이터를 지우고 나머지 데이터를 세팅
			m_recvBuffer.erase(m_recvBuffer.begin(), m_recvBuffer.begin() + m_packetSize);

			// 패킷 크기 초기화
			m_packetSize = m_recvBuffer.size();
		}

		// recv 다시 요청
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

void TCPSession::SendIOCompletion()
{
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


void TCPSession::Send(PacketBase* _packet)
{
	// Serializer 객체에 데이터 직렬화
	Serializer serializer;
	_packet->Serialize(serializer);
	const std::vector<uint8_t>& buffer = serializer.GetBuffer();
	assert(buffer.empty() == false);

	// 직렬화한 데이터의 크기와 
	// 서버에서 한번에 받을 수 있는 크기를 확인하여 
	// 나눠서 Send
	constexpr int IOBUFFER_SIZE = IOCompletionData::BUFFER_SIZE;
	int count = static_cast<int>(buffer.size()) / IOBUFFER_SIZE;
	for (int i = 0; i < count; ++i)
	{
		m_IOCompletionSend.SetBuffer(buffer, IOBUFFER_SIZE * (i), IOBUFFER_SIZE * (i + 1));
		SendIOCompletion();
	}

	if (IOBUFFER_SIZE * count < buffer.size())
	{
		m_IOCompletionSend.SetBuffer(buffer, IOBUFFER_SIZE * count, static_cast<UINT>(buffer.size()));
		SendIOCompletion();
	}
}




