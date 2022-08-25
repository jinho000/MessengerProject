#pragma once


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
};


struct UserInfo
{
	std::string ID;
	std::string PW;
	std::vector<std::string> FriendList;
	std::vector<ChatMessage> UnreadMessage;

	UserInfo()
	{}

	UserInfo(UserInfo& _other)
		: ID(_other.ID)
		, PW(_other.PW)
		, FriendList(std::move(_other.FriendList))
		, UnreadMessage(std::move(_other.UnreadMessage))
	{
	}
};
