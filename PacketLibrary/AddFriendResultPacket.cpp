#include "pch.h"
#include "AddFriendResultPacket.h"
#include "PacketHelper.h"

AddFriendResultPacket::AddFriendResultPacket()
    : PacketBase(PACKET_TYPE::ADD_FRIEND_RESULT)
    , m_addFriendResult(RESULT_TYPE::FAIL)
{
}

AddFriendResultPacket::AddFriendResultPacket(RESULT_TYPE _addFriendResult)
    : PacketBase(PACKET_TYPE::ADD_FRIEND_RESULT)
    , m_addFriendResult(_addFriendResult)
{
}

AddFriendResultPacket::~AddFriendResultPacket()
{
}

size_t AddFriendResultPacket::GetContentPacketSize()
{
    return PacketHelper::GetTypeSize(m_addFriendResult);
}

void AddFriendResultPacket::Serialize(Serializer& _serializer)
{
    PacketBase::Serialize(_serializer);
    _serializer.WriteEnum<RESULT_TYPE>(m_addFriendResult);
}

void AddFriendResultPacket::Deserialize(const Serializer& _serializer)
{
    PacketBase::Deserialize(_serializer);
    _serializer.ReadEnum<RESULT_TYPE>(m_addFriendResult);
}
