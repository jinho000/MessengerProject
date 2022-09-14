#include "pch.h"
#include "LogoutPacket.h"
#include "PacketHelper.h"

LogoutPacket::LogoutPacket()
	: PacketBase(PACKET_TYPE::LOGOUT)
{
}

LogoutPacket::LogoutPacket(const std::string& _logoutID)
	: PacketBase(PACKET_TYPE::LOGOUT)
	, m_logoutID(_logoutID)
{
}

LogoutPacket::~LogoutPacket()
{
}


size_t LogoutPacket::GetContentPacketSize()
{
	return PacketHelper::GetTypeSize(m_logoutID);
}

Serializer LogoutPacket::Serialize()
{
	Serializer serializer = PacketBase::Serialize();
	serializer << m_logoutID;

	return serializer;
}

void LogoutPacket::Deserialize(const Serializer& _serializer)
{
	PacketBase::Deserialize(_serializer);
	_serializer >> m_logoutID;
}
