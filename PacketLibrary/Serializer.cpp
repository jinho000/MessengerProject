#include "pch.h"
#include "Serializer.h"

Serializer::Serializer(size_t _bufferSize)
	: m_offset()
{
	m_buffer.resize(_bufferSize);
}

Serializer::Serializer(const std::vector<uint8_t>& _buffer)
	: m_buffer(_buffer)
	, m_offset()
{
}

Serializer::~Serializer()
{
}

void Serializer::Write(const void* _data, int _size)
{
	// 경계값 체크
	assert(m_offset - 1 + _size < m_buffer.size());

	memcpy_s(&m_buffer[m_offset], _size, _data, _size);
	m_offset += _size;
}

void Serializer::operator<<(int _value)
{
	Write(&_value, sizeof(int));
}

void Serializer::operator<<(bool _value)
{
	Write(&_value, sizeof(bool));
}

void Serializer::operator<<(UINT _value)
{
	Write(&_value, sizeof(UINT));
}

void Serializer::operator<<(size_t _value)
{
	Write(&_value, sizeof(size_t));
}

void Serializer::operator<<(float _value)
{
	Write(&_value, sizeof(float));
}

void Serializer::operator<<(const std::string& _value)
{
	size_t size = _value.size();
	Write(&size, sizeof(size_t));
	Write(_value.c_str(), static_cast<int>(_value.size()));
}

void Serializer::operator<<(const ChatMessage& _value)
{
	operator<<(_value.sendUserID);
	operator<<(_value.recvUserID);
	operator<<(_value.message);
	operator<<(_value.bRead);
}

void Serializer::operator<<(const UserInfo& _value)
{
	operator<<(_value.ID);
	operator<<(_value.PW);
	WriteVector(_value.FriendList);
	WriteVector(_value.UnreadMessage);
}

void Serializer::WritePacketType(PACKET_TYPE _packetType)
{
	WriteEnum<PACKET_TYPE>(_packetType);
}

void Serializer::WritePacketSize(size_t _value)
{
	Write(&_value, sizeof(size_t));
}

void Serializer::Read(void* _data, int _size) const
{
	// 경계값 체크
	assert(m_offset - 1 + _size < m_buffer.size());

	memcpy_s(_data, _size, &m_buffer[m_offset], _size);
	m_offset += _size;
}

void Serializer::ReadPacketType(PACKET_TYPE& _packetType) const
{
	ReadEnum<PACKET_TYPE>(_packetType);
}

void Serializer::ReadPacketSize(size_t& _packetSize) const
{
	Read(&_packetSize, sizeof(size_t));
}

void Serializer::operator>>(int& _value) const
{
	Read(&_value, sizeof(int));
}

void Serializer::operator>>(bool& _value) const
{
	Read(&_value, sizeof(bool));
}

void Serializer::operator>>(UINT& _value) const
{
	Read(&_value, sizeof(UINT));
}

void Serializer::operator>>(size_t& _value) const
{
	Read(&_value, sizeof(size_t));
}

void Serializer::operator>>(float& _value) const
{
	Read(&_value, sizeof(float));
}

void Serializer::operator>>(std::string& _value) const
{
	size_t size = 0;
	Read(&size, sizeof(size_t));
	_value.resize(size);
	Read(&_value[0], static_cast<int>(size));
}

void Serializer::operator>>(ChatMessage& _value) const
{
	operator>>(_value.sendUserID);
	operator>>(_value.recvUserID);
	operator>>(_value.message);
	operator>>(_value.bRead);
}

void Serializer::operator>>(UserInfo& _value) const
{
	operator>>(_value.ID);
	operator>>(_value.PW);
	ReadVector(_value.FriendList);
	ReadVector(_value.UnreadMessage);
}
