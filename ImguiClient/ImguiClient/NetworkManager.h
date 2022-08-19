#pragma once
#include <WinSock2.h>
#include <Windows.h>

#include <thread>
#include <vector>
#include <functional>

#include <PacketLibrary/PacketBase.h>
#include <GameServer2/ClientSocket.h>

using ClientPacketDispatchFunction = std::function<void(std::unique_ptr<PacketBase>)>;

class NetworkManager
{
private: 
	static NetworkManager* pInst;
	
	enum
	{
		RECV_BUFFER_SIZE = 255
	};

	std::thread		m_recvThread;
	ClientSocket*	m_clientSocket; // ���� ���� �� ������ ����

	std::unordered_map<PACKET_TYPE, ClientPacketDispatchFunction> m_clientDispatchFuncion;

private: 
	NetworkManager();
	~NetworkManager();

	NetworkManager(const NetworkManager& _other) = delete;
	NetworkManager(NetworkManager&& _other) = delete;
	NetworkManager& operator=(const NetworkManager& _other) = delete;
	NetworkManager& operator=(const NetworkManager&& _other) = delete;


public: 
	static void CreateIntance();
	static void Destroy();
	static NetworkManager* GetInst() { return pInst; }

private:
	
	void ListenRecv();
	void ListenThread();

public:
	void Send(PacketBase* _packet);
};

