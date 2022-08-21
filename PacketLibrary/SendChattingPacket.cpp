#include "pch.h"
#include "SendChattingPacket.h"
#include "PacketHelper.h"

SendChattingPacket::SendChattingPacket()
	: PacketBase(PACKET_TYPE::SEND_CHATTING)
{
}

SendChattingPacket::SendChattingPacket(const std::string& _sendUserID, const std::string& _recvUserID, const std::string& _chatMessage)
	: PacketBase(PACKET_TYPE::SEND_CHATTING)
	, m_sendUserID(_sendUserID)
	, m_recvUserID(_recvUserID)
	, m_chatMessage(_chatMessage)
{
}

SendChattingPacket::~SendChattingPacket()
{
}

size_t SendChattingPacket::GetContentPacketSize()
{
	return PacketHelper::GetTypeSize(m_sendUserID, m_recvUserID, m_chatMessage);
}

void SendChattingPacket::Serialize(Serializer& _serializer)
{
	PacketBase::Serialize(_serializer);
	_serializer << m_sendUserID;
	_serializer << m_recvUserID;
	_serializer << m_chatMessage;
}

void SendChattingPacket::Deserialize(const Serializer& _serializer)
{
	PacketBase::Deserialize(_serializer);
	_serializer >> m_sendUserID;
	_serializer >> m_recvUserID;
	_serializer >> m_chatMessage;
}
