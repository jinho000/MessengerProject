#pragma once
#include "PacketBase.h"

class SendRecvPacket : public PacketBase
{
public:
	SendRecvPacket(PACKET_TYPE _packetType);
	virtual ~SendRecvPacket() = default;

public:
	virtual void Serialize() = 0;
	virtual void Deserialize() = 0;
};

