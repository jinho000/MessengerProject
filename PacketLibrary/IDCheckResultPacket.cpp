#include "pch.h"
#include "IDCheckResultPacket.h"
#include "PacketHelper.h"


IDCheckResultPacket::IDCheckResultPacket()
    : PacketBase(PACKET_TYPE::IDCHECK_RESULT)
    , m_IDCheckResult(RESULT_TYPE::FAIL)
{
}

IDCheckResultPacket::IDCheckResultPacket(RESULT_TYPE _IDCheckResult)
    : PacketBase(PACKET_TYPE::IDCHECK_RESULT)
    , m_IDCheckResult(_IDCheckResult)
{
}

IDCheckResultPacket::~IDCheckResultPacket()
{
}

size_t IDCheckResultPacket::GetContentPacketSize()
{
    return PacketHelper::GetTypeSize(m_IDCheckResult);
}

Serializer IDCheckResultPacket::Serialize()
{
    Serializer serializer = PacketBase::Serialize();
    serializer.WriteEnum<RESULT_TYPE>(m_IDCheckResult);

    return serializer;
}

void IDCheckResultPacket::Deserialize(const Serializer& _serializer)
{
    PacketBase::Deserialize(_serializer);
    _serializer.ReadEnum<RESULT_TYPE>(m_IDCheckResult);
}
