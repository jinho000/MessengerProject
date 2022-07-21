#include "pch.h"
#include "PacketBase.h"

PacketBase::PacketBase(PACKET_TYPE _packetType)
	: m_packetType(_packetType)
{
	m_packetSize = sizeof(m_packetType) + sizeof(m_packetType);
}

PacketBase::PacketBase(PACKET_TYPE _packetType, std::vector<uint8_t>& _IOBuffer)
	: m_packetType(_packetType)
	, m_packetSize()
	, m_IOBuffer(std::move(_IOBuffer))
{
}

PacketBase::~PacketBase()
{
}
