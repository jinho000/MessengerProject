#pragma once
#include "PacketBase.h"

class SendPacket : public PacketBase
{
public:
	SendPacket(PACKET_TYPE _packetType);
	virtual ~SendPacket() = default;

public:
	virtual void Serialize() = 0;
};

