#pragma once
#include "pch.h"
#include "PacketType.h"

// 데이터 직렬화 처리
// 패킷의 데이터만 버퍼에 저장하여 처리하므로 구조체의 패딩을 고려할 필요 없음
class Serializer
{
private:
	std::vector<uint8_t>	m_buffer;
	mutable size_t			m_offset;

public:
	Serializer();
	Serializer(const std::vector<uint8_t>& _buffer);
	~Serializer();

	Serializer(const Serializer& _other) = delete;
	Serializer(Serializer&& _other) = delete;
	Serializer& operator=(const Serializer& _other) = delete;
	Serializer& operator=(const Serializer&& _other) = delete;

public: 
	// Write
	void Write(const void* _data, int size);
	void WritePacketType(PACKET_TYPE _packetType);
	void WritePacketSize(size_t _value);

	void operator<<(int _value);
	void operator<<(bool _value);
	void operator<<(UINT _value);
	void operator<<(size_t _value);
	void operator<<(float _value);
	void operator<<(const std::string& _value);
	void operator<<(const ChatMessage& _value);
	void operator<<(const UserInfo& _value);


	template<typename T>
	void WriteEnum(const T _Value)
	{
		Write(reinterpret_cast<const void*>(&_Value), static_cast<unsigned int>(sizeof(T)));
	}


	template<typename T>
	void WriteVector(std::vector<T>& _Value)
	{
		operator<<(static_cast<size_t>(_Value.size()));
		for (size_t i = 0; i < _Value.size(); i++)
		{
			operator<<(_Value[i]);
		}
	}

	template<typename T>
	void WriteVector(const std::vector<T>& _Value)
	{
		operator<<(static_cast<size_t>(_Value.size()));
		for (size_t i = 0; i < _Value.size(); i++)
		{
			operator<<(_Value[i]);
		}
	}

	// read
	void Read(void* _data, int size) const;
	void ReadPacketType(PACKET_TYPE& _packetType) const;
	void ReadPacketSize(size_t& _packetSize) const;

	void operator>>(int& _value) const;
	void operator>>(bool& _value) const;
	void operator>>(UINT& _value) const;
	void operator>>(size_t& _value) const;
	void operator>>(float& _value) const;
	void operator>>(std::string& _value) const;
	void operator>>(ChatMessage& _value) const;
	void operator>>(UserInfo& _value) const;

	template<typename T>
	void ReadEnum(T& _Value) const
	{
		Read(reinterpret_cast<void*>(&_Value), static_cast<unsigned int>(sizeof(T)));
	}

	template<typename T>
	void ReadVector(std::vector<T>& _Value) const
	{
		size_t Size;
		operator>>(Size);
		_Value.resize(Size);
		for (size_t i = 0; i < _Value.size(); i++)
		{
			operator>>(_Value[i]);
		}
	}

public:
	void ResizeBuffer(size_t _bufferSize);
	const std::vector<uint8_t>& GetBuffer() { return m_buffer; }

};

