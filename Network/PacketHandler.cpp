#include "pch.h"
#include "PacketHandler.h"
#include <PacketLibrary/PacketHelper.h>
#include <PacketLibrary/PacketBase.h>

#include "DispatchFunction.h"

PacketHandler::PacketHandler()
{
	m_serverDispatchFuncion.resize(static_cast<int>(PACKET_TYPE::SIZE));
	AddDispatchFunction();
}


void PacketHandler::AddDispatchFunction()
{
	m_serverDispatchFuncion[static_cast<int>(PACKET_TYPE::LOGIN)] = DispatchLoginPacket;
	m_serverDispatchFuncion[static_cast<int>(PACKET_TYPE::JOIN)] = DispatchJoinPacket;
	m_serverDispatchFuncion[static_cast<int>(PACKET_TYPE::IDCHECK)] = DispatchIDCheckPacket;
	m_serverDispatchFuncion[static_cast<int>(PACKET_TYPE::ADD_FRIEND)] = DispatchAddFriendPacket;
	m_serverDispatchFuncion[static_cast<int>(PACKET_TYPE::CHATTING)] = DispatchSendChattingPacket;
	m_serverDispatchFuncion[static_cast<int>(PACKET_TYPE::LOGOUT)] = DispatchLogoutPacket;
	m_serverDispatchFuncion[static_cast<int>(PACKET_TYPE::READ_CHATTING)] = DispatchReadChattingPacket;
	m_serverDispatchFuncion[static_cast<int>(PACKET_TYPE::CLIENT_EXIT)] = DispatchClientExitPacket;
}

void PacketHandler::DispatchPacket(TCPSession* _pTCPSession, std::unique_ptr<PacketBase> _packet)
{
	const ServerPacketDispatchFunction& dispatchFunction = m_serverDispatchFuncion[static_cast<int>(_packet->GetPacketType())];
	dispatchFunction(_pTCPSession, std::move(_packet));
}
