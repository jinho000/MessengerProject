#include "pch.h"
#include "JoinPacket.h"
#include "PacketHelper.h"


JoinPacket::JoinPacket()
	: PacketBase(PACKET_TYPE::JOIN)
{
}

JoinPacket::JoinPacket(const std::string& _id, const std::string& _pw)
	: PacketBase(PACKET_TYPE::JOIN)
	, m_joinID(_id)
	, m_joinPW(_pw)
{
}

JoinPacket::~JoinPacket()
{
}

size_t JoinPacket::GetContentPacketSize()
{
	return PacketHelper::GetTypeSize(m_joinID, m_joinPW);
}

void JoinPacket::Serialize(Serializer& _serialize)
{
	PacketBase::Serialize(_serialize);
	_serialize << m_joinID;
	_serialize << m_joinPW;
}

void JoinPacket::Deserialize(const Serializer& _serialize)
{
	PacketBase::Deserialize(_serialize);
	_serialize >> m_joinID;
	_serialize >> m_joinPW;
}
