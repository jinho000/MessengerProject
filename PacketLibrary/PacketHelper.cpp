#include "pch.h"
#include "PacketHelper.h"
#include "PacketType.h"

#include "LoginPacket.h"

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
		break;
	case PACKET_TYPE::JOIN:
		break;
	case PACKET_TYPE::JOIN_RESULT:
		break;
	case PACKET_TYPE::CHATTING:
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
