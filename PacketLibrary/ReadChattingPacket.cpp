#include "pch.h"
#include "ReadChattingPacket.h"
#include "PacketHelper.h"

ReadChattingPacket::ReadChattingPacket()
	: PacketBase(PACKET_TYPE::READ_CHATTING)
	, m_readCount(-1)
{
}

ReadChattingPacket::ReadChattingPacket(const std::string& _sendUserID, const std::string& _recvUserID, int _readCount)
	: PacketBase(PACKET_TYPE::READ_CHATTING)
	, m_sendUserID(_sendUserID)
	, m_recvUserID(_recvUserID)
	, m_readCount(_readCount)
{
}

ReadChattingPacket::~ReadChattingPacket()
{
}

size_t ReadChattingPacket::GetContentPacketSize()
{
	return PacketHelper::GetTypeSize(m_sendUserID, m_recvUserID, m_readCount);
}

Serializer ReadChattingPacket::Serialize()
{
	Serializer serializer = PacketBase::Serialize();
	serializer << m_sendUserID;
	serializer << m_recvUserID;
	serializer << m_readCount;

	return serializer;
}

void ReadChattingPacket::Deserialize(const Serializer& _serializer)
{
	PacketBase::Deserialize(_serializer);
	_serializer >> m_sendUserID;
	_serializer >> m_recvUserID;
	_serializer >> m_readCount;
}
