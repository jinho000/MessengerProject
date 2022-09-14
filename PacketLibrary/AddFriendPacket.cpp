#include "pch.h"
#include "AddFriendPacket.h"
#include "PacketHelper.h"

AddFriendPacket::AddFriendPacket()
	: PacketBase(PACKET_TYPE::ADD_FRIEND)
{
}

AddFriendPacket::AddFriendPacket(const std::string& _userID, const std::string& _friendID)
	: PacketBase(PACKET_TYPE::ADD_FRIEND)
	, m_userID(_userID)
	, m_friendID(_friendID)
{
}

AddFriendPacket::~AddFriendPacket()
{
}

size_t AddFriendPacket::GetContentPacketSize()
{
	return PacketHelper::GetTypeSize(m_userID, m_friendID);
}

Serializer AddFriendPacket::Serialize()
{
	Serializer serializer = PacketBase::Serialize();
	serializer << m_userID;
	serializer << m_friendID;

	return serializer;
}

void AddFriendPacket::Deserialize(const Serializer& _serialize)
{
	PacketBase::Deserialize(_serialize);
	_serialize >> m_userID;
	_serialize >> m_friendID;
}
