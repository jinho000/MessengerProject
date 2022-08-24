#include "pch.h"
#include "PacketHelper.h"
#include "PacketType.h"

#include "PacketHeader.h"

std::unique_ptr<PacketBase> PacketHelper::ConvertToPacket(const std::vector<uint8_t>& _buffer)
{
	PACKET_TYPE type;
	memcpy_s(&type, sizeof(PACKET_TYPE), _buffer.data(), sizeof(PACKET_TYPE));

	std::unique_ptr<PacketBase> pPacket = nullptr;
	switch (type)
	{
	case PACKET_TYPE::LOGIN:
		pPacket = std::make_unique<LoginPacket>();
		break;
	case PACKET_TYPE::LOGIN_RESULT:
		pPacket = std::make_unique<LoginResultPacket>();
		break;
	case PACKET_TYPE::JOIN:
		pPacket = std::make_unique<JoinPacket>();
		break;
	case PACKET_TYPE::JOIN_RESULT:
		pPacket = std::make_unique<JoinResultPacket>();
		break;
	case PACKET_TYPE::CHATTING:
		pPacket = std::make_unique<ChattingPacket>();
		break;
	case PACKET_TYPE::IDCHECK:
		pPacket = std::make_unique<IDCheckPacket>();
		break;
	case PACKET_TYPE::IDCHECK_RESULT:
		pPacket = std::make_unique<IDCheckResultPacket>();
		break;
	case PACKET_TYPE::ADD_FRIEND:
		pPacket = std::make_unique<AddFriendPacket>();
		break;
	case PACKET_TYPE::ADD_FRIEND_RESULT:
		pPacket = std::make_unique<AddFriendResultPacket>();
		break;
	case PACKET_TYPE::LOGOUT:
		pPacket = std::make_unique<LogoutPacket>();
		break;
	case PACKET_TYPE::READ_CHATTING:
		pPacket = std::make_unique<ReadChattingPacket>();
		break;
	default:
		break;
	}

	Serializer serializer(_buffer);
	pPacket->Deserialize(serializer);

	return std::move(pPacket);
}
