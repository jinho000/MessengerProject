#include "pch.h"
#include "RecvChattingPacket.h"
#include "PacketHelper.h"

RecvChattingPacket::RecvChattingPacket()
    : PacketBase(PACKET_TYPE::RECV_CHATTING)
{
}

RecvChattingPacket::RecvChattingPacket(const std::string& _sendUserID, const std::string& _recvUserID, const std::string& _chatMessage)
    : PacketBase(PACKET_TYPE::RECV_CHATTING)
    , m_sendUserID(_sendUserID)
    , m_recvUserID(_recvUserID)
    , m_chatMessage(_chatMessage)
{
}

RecvChattingPacket::~RecvChattingPacket()
{
}

size_t RecvChattingPacket::GetContentPacketSize()
{
    return PacketHelper::GetTypeSize(m_sendUserID, m_recvUserID, m_chatMessage);
}

void RecvChattingPacket::Serialize(Serializer& _serializer)
{
    PacketBase::Serialize(_serializer);
    _serializer << m_sendUserID;
    _serializer << m_recvUserID;
    _serializer << m_chatMessage;
}

void RecvChattingPacket::Deserialize(const Serializer& _serializer)
{
    PacketBase::Deserialize(_serializer);
    _serializer >> m_sendUserID;
    _serializer >> m_recvUserID;
    _serializer >> m_chatMessage;
}
