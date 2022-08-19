#pragma once
#include "Singleton.h"
#include <PacketLibrary/PacketType.h>

class PacketBase;
using ServerPacketDispatchFunction = std::function<void(TCPSession*, std::unique_ptr<PacketBase>)>;
using ClientPacketDispatchFunction = std::function<void(std::unique_ptr<PacketBase>)>;
using ClientCallback = std::function<void(std::unique_ptr<PacketBase>)>;

class PacketHandler : public Singleton<PacketHandler>
{
	friend class Singleton;

private:
	std::unordered_map<PACKET_TYPE, ServerPacketDispatchFunction>	m_serverDispatchFuncion;
	std::unordered_map<PACKET_TYPE, ClientCallback>					m_clientCallbackMap;

private:
	PacketHandler();
	~PacketHandler() = default;

public: // member Func
	void AddClientCallback(PACKET_TYPE _packetType, ClientCallback _clientCallback);

	void DispatchServerPacket(TCPSession* _pTCPSession, const std::vector<uint8_t>& _buffer);
	void DispatchClientPacket(const std::vector<uint8_t>& _buffer);
};

