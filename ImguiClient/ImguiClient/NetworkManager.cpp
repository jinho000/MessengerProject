#include "NetworkManager.h"

#include <Network/ServerHelper.h>
#include <Network/PacketHandler.h>
#include <Network/ClientSocket.h>

#include <PacketLibrary/pch.h>
#include <PacketLibrary/Serializer.h>
#include <PacketLibrary/PacketHelper.h>

#include <cassert>
#include <memory>	

#include "JoinWindow.h"
#include "LoginWindow.h"
#include "MainWindow.h"
#include "ChatWindow.h"
#include "ImguiWindowManager.h"	
#include "ServerConnectModal.h"

NetworkManager* NetworkManager::pInst = nullptr;

NetworkManager::NetworkManager()
	: m_packetSize(0)
	, m_bExit(false)
	, m_bConnected(false)
{
	// ���϶��̺귯�� ����
	ServerHelper::WSAStart();

	// ���϶��̺귯�� ���� �� ���� ����
	m_clientSocket = new ClientSocket(9900, "127.0.0.1", IPPROTO::IPPROTO_TCP);

	// ��Ŷó�� �Լ� �߰�
	AddDispatchFunction();
}

NetworkManager::~NetworkManager()
{
	if (m_bConnected == true)
	{
		while (m_bExit.load() == false)
		{
			Sleep(10);
		}
	}

	delete m_clientSocket;
	m_clientSocket = nullptr;

	if (m_recvThread.joinable())
	{
		m_recvThread.join();
	}

	ServerHelper::WSAEnd();
}

void NetworkManager::CreateIntance()
{
	if (pInst != nullptr)
		return;

	pInst = new NetworkManager;
}

void NetworkManager::Destroy()
{
	if (pInst == nullptr)
		return;

	delete pInst;
	pInst = nullptr;
}

void NetworkManager::AddDispatchFunction()
{
	m_packetHandler.insert(std::make_pair(PACKET_TYPE::JOIN_RESULT, JoinWindow::DispatchJoinResultPacket));
	m_packetHandler.insert(std::make_pair(PACKET_TYPE::IDCHECK_RESULT, JoinWindow::DispatchIDCheckResultPacket));
	m_packetHandler.insert(std::make_pair(PACKET_TYPE::LOGIN_RESULT, LoginWindow::DispatchLoginResultPacket));
	m_packetHandler.insert(std::make_pair(PACKET_TYPE::ADD_FRIEND_RESULT, MainWindow::DispatchAddFriendResultPacket));
	m_packetHandler.insert(std::make_pair(PACKET_TYPE::CHATTING, ChatWindow::DispatchRecvChattingPacket));
	m_packetHandler.insert(std::make_pair(PACKET_TYPE::READ_CHATTING, ChatWindow::DispatchReadMessagePacket));
	m_packetHandler.insert(std::make_pair(PACKET_TYPE::CLIENT_EXIT, MainWindow::DispatchClientExitPacket));


}

void NetworkManager::StartRecvThread()
{
	if (m_recvThread.joinable())
	{
		m_recvThread.join();
	}

	m_recvThread = std::thread(&NetworkManager::ListenThread, this);
}

void NetworkManager::ListenThread()
{
	while (true)
	{
		std::vector<uint8_t> buffer;
		buffer.resize(SERVER_BUFFER_SIZE);

		int transferredBytes = recv(m_clientSocket->GetSocket(), reinterpret_cast<char*>(buffer.data()), buffer.size(), 0);

		// ���� ����ó��
		if (transferredBytes == -1)
		{
			// Ŭ���̾�Ʈ�� ���� 10053(WSAECONNABORTED)
			// ������ ����� ��� 10054(WSAECONNRESET)
			int errorcode = GetLastError();
			if (errorcode == WSAECONNRESET)
			{
				// ���� ���� ��� ����
				ServerConnectModal* pServerConnectModal = static_cast<ServerConnectModal*>(ImguiWindowManager::GetInst()->GetServerConnectModal());
				pServerConnectModal->Active();

				// �α׾ƿ��ϱ�
				MainWindow* pMainWindow = static_cast<MainWindow*>(ImguiWindowManager::GetInst()->GetImguiWindow(WINDOW_UI::LOGIN));
				pMainWindow->MoveLoginWindow();
			}

			break;
		}

		// �����Ͱ� ������ ������ �ð�� ó��
		// TCP�� �����ʹ� ��谡 �������� ����
		// �����Ͱ� ��Ŷ�� �����ŭ �Դ��� Ȯ���ϱ�
		// ���� ������ ������ŭ ���ú� ���� �ڿ� ����
		m_recvBuffer.insert(m_recvBuffer.end(), buffer.begin(), buffer.begin() + transferredBytes);

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

			// ��Ŷó��
			std::unique_ptr<PacketBase> pPacket = PacketHelper::ConvertToPacket(buffer);
			auto iter = m_packetHandler.find(pPacket->GetPacketType());
			assert(iter != m_packetHandler.end());

			const ClientPacketDispatchFunction& dispatchFunction = iter->second;
			m_packetHandlerLock.lock();
			dispatchFunction(std::move(pPacket));
			m_packetHandlerLock.unlock();

			// ��Ŷ ó�� �� ����
			// ����� �����͸� ����� ������ �����͸� ����
			// ��Ŷ ũ�� �ʱ�ȭ
			m_recvBuffer.erase(m_recvBuffer.begin(), m_recvBuffer.begin() + m_packetSize);
			m_packetSize = m_recvBuffer.size();
		}

	}
}

void NetworkManager::Send(PacketBase& _packet)
{
	Serializer serializer = _packet.Serialize();
	std::vector<uint8_t> buffer = serializer.GetBuffer();
	int result = send(m_clientSocket->GetSocket(), reinterpret_cast<char*>(buffer.data()), buffer.size(), 0);
}

bool NetworkManager::ConnectServer()
{
	m_bConnected = m_clientSocket->ConnectServer();
	if (m_bConnected == false)
	{
		return false;
	}

	
	// RecvThread ����
	StartRecvThread();

	return true;
}

bool NetworkManager::ReConnectServer()
{
	if (m_clientSocket != nullptr)
	{
		delete m_clientSocket;
		m_clientSocket = nullptr;
	}

	// ������ �ٽø���� �����Ѵ�
	m_clientSocket = new ClientSocket(9900, "127.0.0.1", IPPROTO::IPPROTO_TCP);

	return ConnectServer();
}

void NetworkManager::SetClientExit()
{
	m_bExit.store(true);
}
