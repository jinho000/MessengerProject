#pragma once
#include <vector>
#include <string>

class ChatFriend;
class User
{
private: // member var
	std::string			m_loginID;
	std::string			m_name;
	
	std::vector<ChatFriend*>	m_chatFriendList;

public: // default
	User(std::string _loginID, std::string _name, std::vector<ChatFriend*>& _chatFriendList);
	~User();

	User(const User& _other) = delete;
	User(User&& _other) = delete;
	User& operator=(const User& _other) = delete;
	User& operator=(const User&& _other) = delete;

public: // member Func
	const std::vector<ChatFriend*>& GetChatFriendList() const { return m_chatFriendList; }
	const std::string GetUserName() { return m_name; }
};

