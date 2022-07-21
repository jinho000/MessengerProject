#include "pch.h"
#include "PacketBase.h"

PacketBase::PacketBase(PACKET_TYPE _packetType)
	: m_packetType(_packetType)
{
	m_packetSize = sizeof(m_packetType) + sizeof(m_packetType);
}

PacketBase::~PacketBase()
{
}
