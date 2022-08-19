#include "pch.h"
#include "PacketHandler.h"
#include <PacketLibrary/PacketHelper.h>
#include <PacketLibrary/PacketBase.h>

#include "LoginPacketHandler.h"

PacketHandler::PacketHandler()
{
	m_serverDispatchFuncion.insert(std::make_pair(PACKET_TYPE::LOGIN, LoginPacketHandler));


}

void PacketHandler::AddClientCallback(PACKET_TYPE _packetType, ClientCallback _clientCallback)
{
	m_clientCallbackMap.insert(std::make_pair(_packetType, _clientCallback));
}

void PacketHandler::DispatchServerPacket(TCPSession* _pTCPSession, const std::vector<uint8_t>& _buffer)
{
	std::unique_ptr<PacketBase> pPacket = PacketHelper::ConvertToPacket(_buffer);
	assert(pPacket != nullptr);

	const ServerPacketDispatchFunction& handlerFunction = m_serverDispatchFuncion.find(pPacket->GetPacketType())->second;
	handlerFunction(_pTCPSession, std::move(pPacket));
}

void PacketHandler::DispatchClientPacket(const std::vector<uint8_t>& _buffer)
{
	std::unique_ptr<PacketBase> pPacket = PacketHelper::ConvertToPacket(_buffer);
	assert(pPacket != nullptr);

	const ClientPacketDispatchFunction& clientCallback = m_clientCallbackMap.find(pPacket->GetPacketType())->second;
	clientCallback(std::move(pPacket));
}


