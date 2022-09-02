#include "pch.h"
#include "PacketHandler.h"
#include <PacketLibrary/PacketHelper.h>
#include <PacketLibrary/PacketBase.h>

#include "DispatchFunction.h"

PacketHandler::PacketHandler()
{
	AddDispatchFunction();
}


void PacketHandler::AddDispatchFunction()
{
	m_serverDispatchFuncion.insert(std::make_pair(PACKET_TYPE::LOGIN, DispatchLoginPacket));
	m_serverDispatchFuncion.insert(std::make_pair(PACKET_TYPE::JOIN, DispatchJoinPacket));
	m_serverDispatchFuncion.insert(std::make_pair(PACKET_TYPE::IDCHECK, DispatchIDCheckPacket));
	m_serverDispatchFuncion.insert(std::make_pair(PACKET_TYPE::ADD_FRIEND, DispatchAddFriendPacket));
	m_serverDispatchFuncion.insert(std::make_pair(PACKET_TYPE::CHATTING, DispatchSendChattingPacket));
	m_serverDispatchFuncion.insert(std::make_pair(PACKET_TYPE::LOGOUT, DispatchLogoutPacket));
	m_serverDispatchFuncion.insert(std::make_pair(PACKET_TYPE::READ_CHATTING, DispatchReadChattingPacket));
}

void PacketHandler::DispatchPacket(TCPSession* _pTCPSession, std::unique_ptr<PacketBase> _packet)
{
	const ServerPacketDispatchFunction& dispatchFunction = m_serverDispatchFuncion.find(_packet->GetPacketType())->second;
	dispatchFunction(_pTCPSession, std::move(_packet));
}
