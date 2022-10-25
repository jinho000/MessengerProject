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
	// 소켓라이브러리 시작
	ServerHelper::WSAStart();

	// 소켓라이브러리 시작 후 소켓 생성
	m_clientSocket = new ClientSocket(9900, "127.0.0.1", IPPROTO::IPPROTO_TCP);

	// 패킷처리 함수 추가
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

		// 소켓 종료처리
		if (transferredBytes == -1)
		{
			// 클라이언트의 종료 10053(WSAECONNABORTED)
			// 서버가 종료된 경우 10054(WSAECONNRESET)
			int errorcode = GetLastError();
			if (errorcode == WSAECONNRESET)
			{
				// 서버 연결 모달 띄우기
				ServerConnectModal* pServerConnectModal = static_cast<ServerConnectModal*>(ImguiWindowManager::GetInst()->GetServerConnectModal());
				pServerConnectModal->Active();

				// 로그아웃하기
				MainWindow* pMainWindow = static_cast<MainWindow*>(ImguiWindowManager::GetInst()->GetImguiWindow(WINDOW_UI::LOGIN));
				pMainWindow->MoveLoginWindow();
			}

			break;
		}

		// 데이터가 여러번 나눠서 올경우 처리
		// TCP의 데이터는 경계가 존재하지 않음
		// 데이터가 패킷의 사이즈만큼 왔는지 확인하기
		// 들어온 데이터 개수만큼 리시브 버퍼 뒤에 저장
		m_recvBuffer.insert(m_recvBuffer.end(), buffer.begin(), buffer.begin() + transferredBytes);

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

			// 패킷처리
			std::unique_ptr<PacketBase> pPacket = PacketHelper::ConvertToPacket(buffer);
			auto iter = m_packetHandler.find(pPacket->GetPacketType());
			assert(iter != m_packetHandler.end());

			const ClientPacketDispatchFunction& dispatchFunction = iter->second;
			m_packetHandlerLock.lock();
			dispatchFunction(std::move(pPacket));
			m_packetHandlerLock.unlock();

			// 패킷 처리 후 세팅
			// 사용한 데이터를 지우고 나머지 데이터를 세팅
			// 패킷 크기 초기화
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

	
	// RecvThread 시작
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

	// 소켓을 다시만들어 연결한다
	m_clientSocket = new ClientSocket(9900, "127.0.0.1", IPPROTO::IPPROTO_TCP);

	return ConnectServer();
}

void NetworkManager::SetClientExit()
{
	m_bExit.store(true);
}
