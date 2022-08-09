#include "pch.h"
#include "PacketBase.h"

PacketBase::PacketBase(PACKET_TYPE _packetType)
	: m_packetType(_packetType)
	, m_packetSize()
{
}

PacketBase::~PacketBase()
{
}

