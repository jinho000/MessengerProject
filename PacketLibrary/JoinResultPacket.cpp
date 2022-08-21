#include "pch.h"
#include "JoinResultPacket.h"
#include "PacketHelper.h"

JoinResultPacket::JoinResultPacket()
    : PacketBase(PACKET_TYPE::JOIN_RESULT)
    , m_joinResult(RESULT_TYPE::FAIL)
{
}

JoinResultPacket::JoinResultPacket(RESULT_TYPE _joinResult)
    : PacketBase(PACKET_TYPE::JOIN_RESULT)
    , m_joinResult(_joinResult)
{
}

JoinResultPacket::~JoinResultPacket()
{
}

size_t JoinResultPacket::GetContentPacketSize()
{
    return PacketHelper::GetTypeSize(m_joinResult);
}

void JoinResultPacket::Serialize(Serializer& _serializer)
{
    PacketBase::Serialize(_serializer);
    _serializer.WriteEnum<RESULT_TYPE>(m_joinResult);
}

void JoinResultPacket::Deserialize(const Serializer& _serializer)
{
    PacketBase::Deserialize(_serializer);
    _serializer.ReadEnum<RESULT_TYPE>(m_joinResult);
}
