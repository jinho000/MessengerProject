#pragma once
#include <vector>
#include <string>


class ChatFriend
{
private: // member var
	std::string 				m_chatFriendName;
	std::vector<std::string>	m_messageList;

public: // default
	ChatFriend(const std::string& _chatFriendName, std::vector<std::string>& _messageList);
	ChatFriend(const std::string& _chatFriendName);
	~ChatFriend() = default;

	ChatFriend(const ChatFriend& _other) = delete;
	ChatFriend(ChatFriend&& _other) = delete;
	ChatFriend& operator=(const ChatFriend& _other) = delete;
	ChatFriend& operator=(const ChatFriend&& _other) = delete;

public: // member Func
	const std::string& GetChatFriendName() { return m_chatFriendName; }
	const std::vector<std::string>& GetMessageList() const { return m_messageList; }
	
};

