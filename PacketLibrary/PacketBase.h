#pragma once


enum class PACKET_TYPE
{
	RECV_PACKET,
	SEND_PACKET,
	MOVE,
	ATTACK,
	BUY,

};

class PacketBase
{
protected:
	PACKET_TYPE				m_packetType;
	UINT					m_packetSize;

	std::vector<uint8_t>	m_IOBuffer;
	
public: // default
	PacketBase(PACKET_TYPE _packetType);
	PacketBase(PACKET_TYPE _packetType, std::vector<uint8_t>& _IOBuffer);
	virtual ~PacketBase() = 0;

public:
	virtual void Serialize() {};
	virtual void Deserialize() {};

public:
	const std::vector<uint8_t>& GetBuffer() { m_IOBuffer; }

};

// Packet p(force, test);
// session.send(p);
// 
// PacketHandler::GetInst()->
//
