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
	m_serverDispatchFuncion.insert(std::make_pair(PACKET_TYPE::SEND_CHATTING, DispatchSendChattingPacket));
	m_serverDispatchFuncion.insert(std::make_pair(PACKET_TYPE::LOGOUT, DispatchLogoutPacket));
}

void PacketHandler::DispatchPacket(TCPSession* _pTCPSession, const std::vector<uint8_t>& _buffer)
{
	std::unique_ptr<PacketBase> pPacket = PacketHelper::ConvertToPacket(_buffer);
	assert(pPacket != nullptr);

	const ServerPacketDispatchFunction& dispatchFunction = m_serverDispatchFuncion.find(pPacket->GetPacketType())->second;
	dispatchFunction(_pTCPSession, std::move(pPacket));
}
