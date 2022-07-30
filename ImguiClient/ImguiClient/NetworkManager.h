#pragma once
#include <WinSock2.h>
#include <Windows.h>

#include <thread>
#include <vector>
#include <functional>

#include <PacketLibrary/PacketBase.h>
#include <GameServer2/ClientSocket.h>

class NetworkManager
{
private: 
	static NetworkManager* pInst;
	
	std::thread		m_recvThread;
	ClientSocket*	m_clientSocket; // 소켓 종료 후 스레드 종료


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

