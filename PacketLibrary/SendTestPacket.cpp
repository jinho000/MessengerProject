#include "pch.h"
#include "SendTestPacket.h"

SendTestPacket::SendTestPacket()
	: PacketBase(PACKET_TYPE::SEND_PACKET)
	, m_sendData()
{
}

SendTestPacket::~SendTestPacket()
{
}

void SendTestPacket::Serialize()
{
	// IOBuffer에 데이터 넣기
	int currPos = 0;

	std::memcpy(&m_packetType, m_IOBuffer.data(), sizeof(m_packetType));
	currPos = sizeof(m_packetType);

	std::memcpy(&m_packetSize, m_IOBuffer.data() + currPos, sizeof(m_packetSize));
	currPos = sizeof(m_packetSize);

	std::memcpy(&m_sendData, m_IOBuffer.data() + currPos, sizeof(m_sendData));

}

void SendTestPacket::Deserialize()
{
}
