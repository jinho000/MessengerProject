#include "pch.h"
#include "IDCheckPacket.h"
#include "PacketHelper.h"

IDCheckPacket::IDCheckPacket()
    : PacketBase(PACKET_TYPE::IDCHECK)
{
}

IDCheckPacket::IDCheckPacket(const std::string& _ID)
    : PacketBase(PACKET_TYPE::IDCHECK)
    , m_ID(_ID)
{
}

IDCheckPacket::~IDCheckPacket()
{
}

size_t IDCheckPacket::GetContentPacketSize()
{
    return PacketHelper::GetTypeSize(m_ID);
}

void IDCheckPacket::Serialize(Serializer& _serialize)
{
    PacketBase::Serialize(_serialize);
    _serialize << m_ID;
}

void IDCheckPacket::Deserialize(const Serializer& _serialize)
{
    PacketBase::Deserialize(_serialize);
    _serialize >> m_ID;
}
