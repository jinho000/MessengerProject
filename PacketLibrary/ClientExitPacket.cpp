#include "pch.h"
#include "ClientExitPacket.h"
#include "PacketHelper.h"

ClientExitPacket::ClientExitPacket()
	: PacketBase(PACKET_TYPE::CLIENT_EXIT)
	, m_bExit(true)
{
}

ClientExitPacket::~ClientExitPacket()
{
}

size_t ClientExitPacket::GetContentPacketSize()
{
	return PacketHelper::GetTypeSize(m_bExit);
}

Serializer ClientExitPacket::Serialize()
{
	Serializer serializer = PacketBase::Serialize();
	serializer << m_bExit;

	return serializer;
}

void ClientExitPacket::Deserialize(const Serializer& _serializer)
{
	PacketBase::Deserialize(_serializer);
	_serializer >> m_bExit;
}
