#include "pch.h"
#include "LoginResultPacket.h"
#include "PacketHelper.h"

LoginResultPacket::LoginResultPacket()
	: PacketBase(PACKET_TYPE::LOGIN_RESULT)
	, m_result(RESULT_TYPE::FAIL)
	, m_userInfo()
{
}

LoginResultPacket::LoginResultPacket(RESULT_TYPE _result)
	: PacketBase(PACKET_TYPE::LOGIN_RESULT)
	, m_result(_result)
	, m_userInfo()
{
}

LoginResultPacket::LoginResultPacket(RESULT_TYPE _result, UserInfo& _userInfo)
	: PacketBase(PACKET_TYPE::LOGIN_RESULT)
	, m_result(_result)
	, m_userInfo(_userInfo)
{
}

LoginResultPacket::~LoginResultPacket()
{
}

size_t LoginResultPacket::GetContentPacketSize()
{
	return PacketHelper::GetTypeSize(m_result, m_userInfo.ID, m_userInfo.PW, m_userInfo.FriendList);
}

void LoginResultPacket::Serialize(Serializer& _serializer)
{
	PacketBase::Serialize(_serializer);
	_serializer.WriteEnum<RESULT_TYPE>(m_result);
	_serializer << m_userInfo;
}

void LoginResultPacket::Deserialize(const Serializer& _serializer)
{
	PacketBase::Deserialize(_serializer);
	_serializer.ReadEnum<RESULT_TYPE>(m_result);
	_serializer >> m_userInfo;
}
