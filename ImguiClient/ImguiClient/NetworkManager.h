#pragma once
#include <WinSock2.h>
#include <Windows.h>

#include <thread>
#include <vector>
#include <mutex>

#include <functional>
#include <PacketLibrary/PacketBase.h>

using ClientPacketDispatchFunction = std::function<void(std::unique_ptr<PacketBase>)>;


class ClientSocket;

class NetworkManager
{
private: 
	static NetworkManager* pInst;
	
	enum
	{
		// 서버가 한번에 처리하는 버퍼의 크기는 1024
		SERVER_BUFFER_SIZE = 1024
	};

	std::thread		m_recvThread;
	ClientSocket*	m_clientSocket; // 소켓 종료 후 스레드 종료

	std::unordered_map<PACKET_TYPE, ClientPacketDispatchFunction>	m_packetHandler;
	std::mutex														m_packetHandlerLock;

	
	std::vector<uint8_t>	m_recvBuffer;
	size_t					m_packetSize;

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
	void StartRecvThread();
	void ListenThread();
	void AddDispatchFunction();

public:
	void Send(PacketBase& _packet);
	bool ConnectServer();
	bool ReConnectServer();
};

