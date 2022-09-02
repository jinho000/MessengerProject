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
		// client ���� ���� ó��
		if (_transferredBytes == 0)
		{
			// ���� ����
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

		// TCP�� �����ʹ� ��谡 �������� ����
		// �����Ͱ� ��Ŷ�� �����ŭ �Դ��� Ȯ���ϱ�
		// ���� ������ ������ŭ ���ú� ���� �ڿ� ����
		m_recvBuffer.insert(m_recvBuffer.end(), m_IOCompletionRecv.buffer, m_IOCompletionRecv.buffer + _transferredBytes);

		// ���ú���ۿ� ��Ŷ����� �����Ͱ� ���Դ��� Ȯ��
		if (PacketBase::SIZEOF_PACKET_HEADER < m_recvBuffer.size())
		{
			m_packetSize = *reinterpret_cast<int*>(m_recvBuffer.data() + sizeof(PACKET_TYPE));
		}

		// ���ú���ۿ� ��ü ��Ŷ�����Ͱ� ���Դ��� Ȯ��
		if (m_packetSize <= m_recvBuffer.size())
		{
			// m_recvBuffer���� ��Ŷũ�⸸ŭ �����͸� ������ ���ۿ� ä��
			std::vector<uint8_t> buffer;
			buffer.assign(m_recvBuffer.begin(), m_recvBuffer.begin() + m_packetSize);
	
			// ���� ��Ŷ ó��
			std::unique_ptr<PacketBase> pPacket = PacketHelper::ConvertToPacket(buffer);
			assert(pPacket != nullptr);
			PacketHandler::GetInst()->DispatchPacket(this, std::move(pPacket));

			// ����� �����͸� ����� ������ �����͸� ����
			m_recvBuffer.erase(m_recvBuffer.begin(), m_recvBuffer.begin() + m_packetSize);

			// ��Ŷ ũ�� �ʱ�ȭ
			m_packetSize = m_recvBuffer.size();
		}

		// recv �ٽ� ��û
		RequestRecv();

		break;
	}
	case IOTYPE::SEND:
	{
		break;
	}
	case IOTYPE::DISCONNECT:
	{
		// ���� �Ŵ������� ������
		SessionManager::GetInst()->PopTCPSession(this);

		// ���� Ǯ�� �ֱ� 
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
	// ���Ǽ����� IOCP�� ��� �ƴ��� ���η� ����� �������� �Ǵ�
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
		// �ѹ��� ��ϵ��� ���� ���ϸ� ���
		m_sessionSocket.SetRegistIOCP();
		IOCP::GetInst()->RegisterSocket(m_sessionSocket.GetSocket(), &m_IOCompletionCallback);
	}
}

void TCPSession::RequestRecv()
{
	DWORD recvByte = 0;
	DWORD dwFlags = 0;

	// ���ú� ��û�� �� ������ IOCompletionData ����
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
	// Serializer ��ü�� ������ ����ȭ
	Serializer serializer;
	_packet->Serialize(serializer);
	const std::vector<uint8_t>& buffer = serializer.GetBuffer();
	assert(buffer.empty() == false);

	// ����ȭ�� �������� ũ��� 
	// �������� �ѹ��� ���� �� �ִ� ũ�⸦ Ȯ���Ͽ� 
	// ������ Send
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




