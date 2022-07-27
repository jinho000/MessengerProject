#pragma once
#include <vector>
#include <string>
#include "ChatMessage.h"

class ChatFriend
{
private: // member var
	std::string 				m_chatFriendName;
	std::vector<ChatMessage>	m_messageList;

public: // default
	ChatFriend(const std::string& _chatFriendName, std::vector<ChatMessage>& _messageList);
	ChatFriend(const std::string& _chatFriendName);
	~ChatFriend() = default;

	ChatFriend(const ChatFriend& _other) = delete;
	ChatFriend(ChatFriend&& _other) = delete;
	ChatFriend& operator=(const ChatFriend& _other) = delete;
	ChatFriend& operator=(const ChatFriend&& _other) = delete;

public: // member Func
	const std::string& GetChatFriendName() { return m_chatFriendName; }
	const std::vector<ChatMessage>& GetMessageList() const { return m_messageList; }
	
};

