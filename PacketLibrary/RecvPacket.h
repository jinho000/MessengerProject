#pragma once
#include "PacketBase.h"

class RecvPacket : public PacketBase
{
public:
	RecvPacket(PACKET_TYPE _packetType);
	virtual ~RecvPacket() = default;

public:
	virtual void Deserialize() = 0;
};

