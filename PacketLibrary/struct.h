#pragma once
#include "pch.h"
#include "Serializer.h"

struct ChatMessage
{
	std::string ID;
	std::string message;
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
