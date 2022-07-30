#include "NetworkManager.h"

NetworkManager* NetworkManager::pInst = nullptr;

NetworkManager::NetworkManager()
	: m_clientSocket(new ClientSocket(9900, "127.0.0.1", IPPROTO::IPPROTO_TCP))
{
	ListenRecv();
}

NetworkManager::~NetworkManager()
{
	delete m_clientSocket;
	m_clientSocket = nullptr;

	m_recvThread.join();
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
		buffer.resize(255);

		int result = recv(m_clientSocket->GetSocket(), reinterpret_cast<char*>(buffer.data()), buffer.size(), 0);
		
		// 패킷 설정
		// 패킷을 전달 후 가져간 곳에서 캐스팅하여 사용
		PacketBase* pPacketBase = new PacketBase(buffer);
		pPacketBase->Deserialize();

		// handler처리
		
	}
}

void NetworkManager::Send(PacketBase* _packet)
{
	std::vector<uint8_t> buffer = _packet->GetBuffer();
	int result = send(m_clientSocket->GetSocket(), reinterpret_cast<char*>(buffer.data()), buffer.size(), 0);

}
