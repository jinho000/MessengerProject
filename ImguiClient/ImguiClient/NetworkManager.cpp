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

// connect Error popup
// 		//	ImGui::OpenPopup("Error! ##ConnectServerFail");
	//	// Always center this window when appearing
	//	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	//	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
//if (ImGui::BeginPopupModal("Error! ##ConnectServerFail", NULL, ImGuiWindowFlags_AlwaysAutoResize))
//{
//	ImGui::Text("Connect Server Fail\n\n");
//	ImGui::Separator();

//	if (ImGui::Button("OK", ImVec2(120, 0))) 
//	{ 
//		ImGui::CloseCurrentPopup(); 
//	}

//	ImGui::EndPopup();
//}

NetworkManager* NetworkManager::pInst = nullptr;

NetworkManager::NetworkManager()
{
	ServerHelper::WSAStart();

	// 처음 시작시 서버에 연결 (연결실패처리 해야함)
	m_clientSocket = new ClientSocket(9900, "127.0.0.1", IPPROTO::IPPROTO_TCP);
	m_clientSocket->ConnectServer();
	
	// 패킷처리 함수 추가
	AddDispatchFunction();
}

NetworkManager::~NetworkManager()
{
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


}

void NetworkManager::ListenRecv()
{
	m_recvThread = std::thread(&NetworkManager::ListenThread, this);
}

void NetworkManager::ListenThread()
{
	while (true)
	{
		std::vector<uint8_t> buffer;
		buffer.resize(RECV_BUFFER_SIZE);

		int result = recv(m_clientSocket->GetSocket(), reinterpret_cast<char*>(buffer.data()), buffer.size(), 0);

		// 접속 종료 처리


		// 패킷처리
		std::unique_ptr<PacketBase> pPacket = PacketHelper::ConvertToPacket(buffer);
		auto iter = m_packetHandler.find(pPacket->GetPacketType());
		assert(iter != m_packetHandler.end());

		const ClientPacketDispatchFunction& dispatchFunction = iter->second;
		dispatchFunction(std::move(pPacket));
	}
}

void NetworkManager::Send(PacketBase* _packet)
{
	Serializer serializer(255);
	_packet->Serialize(serializer);
	std::vector<uint8_t> buffer = serializer.GetBuffer();
	int result = send(m_clientSocket->GetSocket(), reinterpret_cast<char*>(buffer.data()), buffer.size(), 0);
}

bool NetworkManager::ConnectServer()
{
	if (m_clientSocket->ConnectServer() == false)
	{
		return false;
	}

	ListenRecv();

	return true;
}
