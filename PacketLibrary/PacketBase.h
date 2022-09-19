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
	virtual ~PacketBase() = 0;

	PacketBase(const PacketBase& _other) = delete;
	PacketBase(PacketBase&& _other) = delete;
	PacketBase& operator=(const PacketBase& _other) = delete;
	PacketBase& operator=(const PacketBase&& _other) = delete;

protected:
	virtual size_t GetContentPacketSize() = 0;

public:
	enum { SIZEOF_PACKET_HEADER = sizeof(PACKET_TYPE) + sizeof(size_t) };

	virtual Serializer Serialize() = 0
	{
		Serializer serializer;
		size_t bufferSize = sizeof(m_packetType) + sizeof(m_packetSize) + GetContentPacketSize();
		serializer.ResizeBuffer(bufferSize);

		serializer.WritePacketType(m_packetType);
		serializer.WritePacketSize(bufferSize);

		return serializer;
	}

	virtual void Deserialize(const Serializer& _serializer) = 0
	{
		_serializer.ReadPacketType(m_packetType);
		_serializer.ReadPacketSize(m_packetSize);
	}

	PACKET_TYPE GetPacketType() { return m_packetType; }
};
