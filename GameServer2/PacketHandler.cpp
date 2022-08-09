#include "pch.h"
#include "PacketHandler.h"
#include <PacketLibrary/PacketHelper.h>
#include <PacketLibrary/PacketBase.h>

#include "LoginPacketHandler.h"

PacketHandler::PacketHandler()
{
	m_dispatchFuncion.insert(std::make_pair(PACKET_TYPE::LOGIN, LoginPacketHandler));

}

void PacketHandler::Dispatch(TCPSession* _pTCPSession, const std::vector<uint8_t>& _buffer)
{
	PacketBase* pPacket = PacketHelper::ConvertToPacket(_buffer);
	assert(pPacket != nullptr);

	std::function<void(TCPSession*, PacketBase*)>& handlerFunction = m_dispatchFuncion.find(pPacket->GetPacketType())->second;
	handlerFunction(_pTCPSession, pPacket);

}
