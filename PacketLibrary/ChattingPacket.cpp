#include "pch.h"
#include "ChattingPacket.h"
#include "PacketHelper.h"

ChattingPacket::ChattingPacket()
	: PacketBase(PACKET_TYPE::CHATTING)
{
}

ChattingPacket::ChattingPacket(const ChatMessage& _chatMessage)
	: PacketBase(PACKET_TYPE::CHATTING)
	, m_chatMessage(_chatMessage)
{
}

ChattingPacket::~ChattingPacket()
{
}

size_t ChattingPacket::GetContentPacketSize()
{
	return PacketHelper::GetTypeSize(m_chatMessage.sendUserID, m_chatMessage.recvUserID, m_chatMessage.message, m_chatMessage.bRead);
}

Serializer ChattingPacket::Serialize()
{
	Serializer serializer = PacketBase::Serialize();
	serializer << m_chatMessage;

	return serializer;
}

void ChattingPacket::Deserialize(const Serializer& _serializer)
{
	PacketBase::Deserialize(_serializer);
	_serializer >> m_chatMessage;
}
