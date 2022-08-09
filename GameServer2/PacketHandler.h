#pragma once
#include "Singleton.h"
#include <PacketLibrary/PacketType.h>

class PacketBase;
using packetDispatchFunction = std::function<void(TCPSession*, PacketBase*)>;

class PacketHandler : public Singleton<PacketHandler>
{
	friend class Singleton;

private:
	std::unordered_map<PACKET_TYPE, packetDispatchFunction> m_dispatchFuncion;

private:
	PacketHandler();
	~PacketHandler() = default;

public: // member Func
	void Dispatch(TCPSession* _pTCPSession, const std::vector<uint8_t>& _buffer);
};

