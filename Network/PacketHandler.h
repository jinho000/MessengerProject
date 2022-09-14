#pragma once
#include "Singleton.h"
#include <PacketLibrary/PacketType.h>

class PacketBase;
class TCPSession;
using ServerPacketDispatchFunction = std::function<void(TCPSession*, std::unique_ptr<PacketBase>)>;
using ClientCallback = std::function<void(std::unique_ptr<PacketBase>)>;

class PacketHandler : public Singleton<PacketHandler>
{
	friend class Singleton;

private:
	//std::unordered_map<PACKET_TYPE, ServerPacketDispatchFunction>	m_serverDispatchFuncion;
	std::vector<ServerPacketDispatchFunction> m_serverDispatchFuncion;

private:
	PacketHandler();
	~PacketHandler() = default;

private:
	void AddDispatchFunction();

public: // member Func
	void DispatchPacket(TCPSession* _pTCPSession, std::unique_ptr<PacketBase> _packet);
};

