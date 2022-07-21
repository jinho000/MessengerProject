#include "pch.h"
#include "RecvTestPacket.h"

RecvTestPacket::RecvTestPacket(std::vector<uint8_t>& _IOBuffer)
	: PacketBase(PACKET_TYPE::RECV_PACKET)
	, m_test()
{
	Deserialize();
}

void RecvTestPacket::Deserialize()
{
	// 버퍼의 데이터를 세팅
	int currPos = 0;

	std::memcpy(m_IOBuffer.data(), &m_packetType, sizeof(m_packetType));
	currPos = sizeof(m_packetType);

	std::memcpy(m_IOBuffer.data() + currPos, &m_packetSize, sizeof(m_packetSize));
	currPos = sizeof(m_packetSize);

	std::memcpy(m_IOBuffer.data() + currPos, &m_test, sizeof(m_test));
}

void RecvTestPacket::Serialize()
{
}
