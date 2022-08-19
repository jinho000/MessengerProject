#include "NetworkManager.h"

#include <Network/ServerHelper.h>
#include <Network/PacketHandler.h>
#include <Network/ClientSocket.h>

#include <PacketLibrary/pch.h>
#include <PacketLibrary/Serializer.h>
#include <PacketLibrary/PacketHelper.h>

#include <cassert>
#include <memory>	

#include "LoginResultPacketHandler.h"

NetworkManager* NetworkManager::pInst = nullptr;

NetworkManager::NetworkManager()
{
	ServerHelper::WSAStart();

	// 패킷처리 함수 추가
	m_clientDispatchFuncion.insert(std::make_pair(PACKET_TYPE::LOGIN_RESULT, LoginResultPacketHandler));

	m_clientSocket = new ClientSocket(9900, "127.0.0.1", IPPROTO::IPPROTO_TCP);
}

NetworkManager::~NetworkManager()
{
	delete m_clientSocket;
	m_clientSocket = nullptr;

	m_recvThread.join();

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
		
		// 패킷처리
		std::unique_ptr<PacketBase> pPacket = PacketHelper::ConvertToPacket(buffer);
		auto iter = m_clientDispatchFuncion.find(pPacket->GetPacketType());
		assert(iter != m_clientDispatchFuncion.end());

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

void NetworkManager::ConnectServer()
{
	assert(m_clientSocket->ConnectServer() == true);

	ListenRecv();
}
