#pragma once


enum class PACKET_TYPE
{
	TEST_PACKET
};

class PacketBase
{
protected:
	PACKET_TYPE				m_packetType;
	UINT					m_packetSize;

	std::vector<uint8_t>	m_IOBuffer;
	UINT					m_offset;
	
public: // default
	PacketBase(PACKET_TYPE _packetType);
	virtual ~PacketBase() = 0;

public:
	const std::vector<uint8_t>& GetBuffer() { m_IOBuffer; }

};
