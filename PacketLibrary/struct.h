#pragma once
#include "pch.h"
#include "Serializer.h"

struct ChatMessage
{
	std::string sendUserID;
	std::string recvUserID;
	std::string message;

	bool		bRead = false;

	ChatMessage() = default;

	ChatMessage(const std::string& _sendUserID, const std::string& _recvUserID, const std::string& _message)
		: sendUserID(_sendUserID)
		, recvUserID(_recvUserID)
		, message(_message)
		, bRead(false)
	{}

	void Serialize(Serializer& _serializer)
	{
		_serializer << sendUserID;
		_serializer << recvUserID;
		_serializer << message;
		_serializer << bRead;
	}

	void Deserialize(const Serializer& _serializer)
	{
		_serializer >> sendUserID;
		_serializer >> recvUserID;
		_serializer >> message;
		_serializer >> bRead;
	}
};


struct UserInfo
{
	std::string ID;
	std::string PW;
	std::vector<std::string> FriendList;

	UserInfo()
	{}

	UserInfo(UserInfo& _other)
		: ID(_other.ID)
		, PW(_other.PW)
	{
		FriendList = std::move(_other.FriendList);
	}

	void Serialize(Serializer& _serializer)
	{
		_serializer << ID;
		_serializer << PW;
		_serializer.WriteVector(FriendList);
	}

	void Deserialize(const Serializer& _serializer)
	{
		_serializer >> ID;
		_serializer >> PW;
		_serializer.ReadVector(FriendList);
	}
};
