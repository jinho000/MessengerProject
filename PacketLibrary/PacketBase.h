#pragma once
#include "PacketType.h"

class PacketBase
{
protected:
	PACKET_TYPE				m_packetType;
	UINT					m_packetSize;

	std::vector<uint8_t>	m_IOBuffer;
	
public: // default
	PacketBase(PACKET_TYPE _packetType);
	PacketBase(std::vector<uint8_t>& _IOBuffer);
	virtual ~PacketBase();

public:
	virtual void Serialize() {};
	virtual void Deserialize() {};

public:
	const std::vector<uint8_t>& GetBuffer() { return m_IOBuffer; }

};

// Packet p(force, test);
// session.send(p);
// 
// PacketHandler::GetInst()->
//
