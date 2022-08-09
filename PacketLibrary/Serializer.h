#pragma once
#include "PacketType.h"

// 单捞磐 流纺拳 贸府
class Serializer
{
private:
	std::vector<uint8_t>	m_buffer;
	mutable size_t			m_offset;

public:
	Serializer();
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
	void operator<<(UINT _value);
	void operator<<(size_t _value);
	void operator<<(float _value);
	void operator<<(const std::string& _value);


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
			_Value[i].Serialize(*this);
		}
	}

	// read
	void Read(void* _data, int size) const;
	void ReadPacketType(PACKET_TYPE& _packetType) const;
	void ReadPacketSize(size_t& _packetSize) const;

	void operator>>(int& _value) const;
	void operator>>(UINT& _value) const;
	void operator>>(size_t& _value) const;
	void operator>>(float& _value) const;
	void operator>>(std::string& _value) const;

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
			_Value[i].Deserialize(*this);
		}
	}

public:
	const std::vector<uint8_t>& GetBuffer() { return m_buffer; }

};

