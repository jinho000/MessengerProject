#include "pch.h"
#include "PacketHelper.h"
#include "PacketType.h"

#include "LoginPacket.h"
#include "LoginResultPacket.h"
#include "JoinPacket.h"
#include "JoinResultPacket.h"
#include "SendChattingPacket.h"
#include "RecvChattingPacket.h"
#include "IDCheckPacket.h"
#include "IDCheckResultPacket.h"
#include "AddFriendPacket.h"
#include "AddFriendResultPacket.h"

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
	case PACKET_TYPE::SEND_CHATTING:
		pPacket = std::make_unique<SendChattingPacket>();
		break;
	case PACKET_TYPE::RECV_CHATTING:
		pPacket = std::make_unique<RecvChattingPacket>();
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
	default:
		break;
	}

	Serializer serializer(_buffer);
	pPacket->Deserialize(serializer);

	return std::move(pPacket);
}

//template<>
//size_t PacketHelper::GetTypeSize(const std::string& _type)
//{
//	return sizeof(_type.size()) + _type.size();
//}
//
//template<typename Type>
//size_t PacketHelper::GetTypeSize(const std::vector<Type>& _Value)
//{
//	int Size = sizeof(_Value.size());
//	for (size_t i = 0; i < _Value.size(); i++)
//	{
//		Size += GetTypeSize(_Value[i]);
//	}
//
//	return Size;
//}
