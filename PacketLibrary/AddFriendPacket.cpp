#include "pch.h"
#include "AddFriendPacket.h"
#include "PacketHelper.h"

AddFriendPacket::AddFriendPacket()
	: PacketBase(PACKET_TYPE::ADD_FRIEND)
{
}

AddFriendPacket::AddFriendPacket(const std::string& _friendID)
	: PacketBase(PACKET_TYPE::ADD_FRIEND)
	, m_friendID(_friendID)
{
}

AddFriendPacket::~AddFriendPacket()
{
}

size_t AddFriendPacket::GetContentPacketSize()
{
	return PacketHelper::GetTypeSize(m_friendID);
}

void AddFriendPacket::Serialize(Serializer& _serialize)
{
	PacketBase::Serialize(_serialize);
	_serialize << m_friendID;
}

void AddFriendPacket::Deserialize(const Serializer& _serialize)
{
	PacketBase::Deserialize(_serialize);
	_serialize >> m_friendID;
}
