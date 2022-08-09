#include "pch.h"
#include "LoginPacket.h"


LoginPacket::LoginPacket(const std::string& _ID, const std::string& _password)
	: PacketBase(PACKET_TYPE::LOGIN)
	, m_ID(_ID)
	, m_password(_password)
{
}

LoginPacket::~LoginPacket()
{
}

size_t LoginPacket::GetContentPacketSize()
{
	return PacketHelper::GetTypeSize(m_ID, m_password);
}

void LoginPacket::Serialize(Serializer& _serialize)
{
	PacketBase::Serialize(_serialize);
	_serialize << m_ID;
	_serialize << m_password;
}

void LoginPacket::Deserialize(const Serializer& _serialize)
{
	PacketBase::Deserialize(_serialize);
	_serialize >> m_ID;
	_serialize >> m_password;
}

