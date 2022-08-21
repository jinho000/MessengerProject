#pragma once
#include <vector>
#include <string>

class ChatFriend;
class User
{
private: // member var
	std::string					m_loginID;
	std::vector<std::string>	m_friendList;

public: // default
	User(std::string _loginID, std::vector<std::string>& _friendList);
	~User();

	User(const User& _other) = delete;
	User(User&& _other) = delete;
	User& operator=(const User& _other) = delete;
	User& operator=(const User&& _other) = delete;

public: // member Func
	const std::vector<std::string>& GetChatFriendList() const { return m_friendList; }
	const std::string& GetUserID() const { return m_loginID; }
};

