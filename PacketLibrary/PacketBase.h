#pragma once
#include "PacketType.h"
#include "Serializer.h"

class PacketBase
{
protected:
	PACKET_TYPE				m_packetType;
	size_t					m_packetSize;

public: // default
	PacketBase(PACKET_TYPE _packetType);
	virtual ~PacketBase();

	PacketBase(const PacketBase& _other) = delete;
	PacketBase(PacketBase&& _other) = delete;
	PacketBase& operator=(const PacketBase& _other) = delete;
	PacketBase& operator=(const PacketBase&& _other) = delete;

protected:
	virtual size_t GetContentPacketSize() = 0;

public:
	virtual void Serialize(Serializer& _serializer) = 0
	{
		_serializer.WritePacketType(m_packetType);
		_serializer.WritePacketSize(sizeof(m_packetType) + sizeof(m_packetSize) + GetContentPacketSize());
	}
	virtual void Deserialize(const Serializer& _serializer) = 0
	{
		_serializer.ReadPacketType(m_packetType);
		_serializer.ReadPacketSize(m_packetSize);
	}

	PACKET_TYPE GetPacketType() { return m_packetType; }
};
