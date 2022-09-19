#include <iostream>
#include <PacketLibrary/PacketHeader.h>
#include <Network/ServerHelper.h>
#include <Network/ClientSocket.h>
#include <thread>
#include <Windows.h>

using namespace std;

constexpr int threadCount = 8;
constexpr int ClientCount = 8;

struct ClientInfo
{
	string ID;
	string PW;
};

ClientInfo clientInfoArry[ClientCount] =
{
	{"ID1", "1"},
	{"ID2", "2"},
	{"ID3", "3"},
	{"ID4", "4"},
	{"ID5", "5"},
	{"ID6", "6"},
	{"ID7", "7"},
	{"ID8", "8"},
};

bool NetworkProcess(PacketBase& _packet, SOCKET _socket)
{
	Serializer serializer = _packet.Serialize();

	std::vector<uint8_t> buffer = serializer.GetBuffer();
	int r = send(_socket, reinterpret_cast<char*>(buffer.data()), buffer.size(), 0);

	r = recv(_socket, (char*)buffer.data(), buffer.size(), 0);
	//cout << "result \n";

	if (r != -1)
		return true;

	return false;
}

void ClientWork(int _clientID)
{
	// 서버 접속하기
	ClientSocket socket(9900, "127.0.0.1", IPPROTO::IPPROTO_TCP);
	
	while (socket.ConnectServer() == false)
	{
		cout << "connect Fail reconnect after 1 second\n";
		Sleep(1000);
	}

	bool bSuccess = true;
	while (bSuccess == true)
	{
		// 서버에 로그인하기
		LoginPacket loginPacket(clientInfoArry[_clientID].ID, clientInfoArry[_clientID].PW);
		bSuccess = NetworkProcess(loginPacket, socket.GetSocket());
		// 서버에서 로그아웃
		LogoutPacket logoutPacket(clientInfoArry[_clientID].ID);
		bSuccess = NetworkProcess(loginPacket, socket.GetSocket());
	}

}



int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	ServerHelper::WSAStart();

	std::thread threadArray[threadCount];
	for (int i = 0; i < threadCount; ++i)
	{
		threadArray[i] = std::thread(&ClientWork, i);
	}

	for (int i = 0; i < threadCount; ++i)
	{
		threadArray[i].join();
	}

	//ClientSocket socket(80, "223.130.195.95", IPPROTO::IPPROTO_TCP);
	//socket.ConnectServer();

	//while (true)
	//{	
	//	char buff[2048] = {};
	//	
	//	int res = recv(socket.GetSocket(), buff, 2048, 0);

	//	if (res == 0)
	//	{
	//		cout << "close" << endl;
	//		break;
	//	}

	//	cout << buff << endl;
	//}

	ServerHelper::WSAEnd();

	return 0;
}
