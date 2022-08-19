#pragma once

class PacketBase;

namespace PacketHelper
{
	template<typename Type>
	size_t GetTypeSize(const Type& _type)
	{
		return sizeof(_type);
	}

	template<>
	size_t GetTypeSize(const std::string& _type);

	template<typename Type>
	size_t GetTypeSize(const std::vector<Type>& _Value);

	template<typename Type, typename ... Types>
	size_t GetTypeSize(const Type& _type, Types... args)
	{
		return GetTypeSize(_type) + GetTypeSize(args...);
	}

	std::unique_ptr<PacketBase> ConvertToPacket(const std::vector<uint8_t>& _buffer);
}
