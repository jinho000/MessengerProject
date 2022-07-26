#include "ChatFriend.h"


ChatFriend::ChatFriend(const std::string& _chatFriendName, std::vector<std::string>& _messageList)
	: m_chatFriendName(_chatFriendName)
	, m_messageList(std::move(_messageList))
{
}

ChatFriend::ChatFriend(const std::string& _chatFriendName)
	: m_chatFriendName(_chatFriendName)
{
}
