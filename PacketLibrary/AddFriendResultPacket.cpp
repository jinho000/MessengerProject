#include "pch.h"
#include "AddFriendResultPacket.h"
#include "PacketHelper.h"

AddFriendResultPacket::AddFriendResultPacket()
    : PacketBase(PACKET_TYPE::ADD_FRIEND_RESULT)
    , m_addFriendResult(RESULT_TYPE::FAIL)
{
}

AddFriendResultPacket::AddFriendResultPacket(const std::string& _friendID, RESULT_TYPE _addFriendResult)
    : PacketBase(PACKET_TYPE::ADD_FRIEND_RESULT)
    , m_friendID(_friendID)
    , m_addFriendResult(_addFriendResult)
{
}

AddFriendResultPacket::~AddFriendResultPacket()
{
}

size_t AddFriendResultPacket::GetContentPacketSize()
{
    return PacketHelper::GetTypeSize(m_friendID, m_addFriendResult);
}

Serializer AddFriendResultPacket::Serialize()
{
    Serializer serializer = PacketBase::Serialize();
    serializer << m_friendID;
    serializer.WriteEnum<RESULT_TYPE>(m_addFriendResult);

    return serializer;
}

void AddFriendResultPacket::Deserialize(const Serializer& _serializer)
{
    PacketBase::Deserialize(_serializer);
    _serializer >> m_friendID;
    _serializer.ReadEnum<RESULT_TYPE>(m_addFriendResult);
}
