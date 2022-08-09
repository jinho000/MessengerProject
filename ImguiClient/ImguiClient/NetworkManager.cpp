#include "NetworkManager.h"
#include <PacketLibrary/Serializer.h>

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
		
		// ��Ŷ ����
		// ��Ŷ�� ���� �� ������ ������ ĳ�����Ͽ� ���
		//PacketBase* pPacketBase = new PacketBase(buffer);
		//pPacketBase->Deserialize();

		// handleró��
		
	}
}

void NetworkManager::Send(PacketBase* _packet)
{
	Serializer serializer;
	_packet->Serialize(serializer);
	std::vector<uint8_t> buffer = serializer.GetBuffer();

	int result = send(m_clientSocket->GetSocket(), reinterpret_cast<char*>(buffer.data()), buffer.size(), 0);

}
