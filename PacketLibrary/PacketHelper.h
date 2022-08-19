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
	inline size_t GetTypeSize(const std::string& _type)
	{
		return sizeof(_type.size()) + _type.size();
	}


	template<typename Type>
	inline size_t GetTypeSize(const std::vector<Type>& _Value)
	{
		int Size = sizeof(_Value.size());
		for (size_t i = 0; i < _Value.size(); i++)
		{
			Size += GetTypeSize(_Value[i]);
		}

		return Size;
	}

	template<typename Type, typename ... Types>
	inline size_t GetTypeSize(const Type& _type, Types... args)
	{
		return GetTypeSize(_type) + GetTypeSize(args...);
	}

	std::unique_ptr<PacketBase> ConvertToPacket(const std::vector<uint8_t>& _buffer);
}
