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
	void AddFriend(const std::string& _friendID) { m_friendList.push_back(_friendID); }

	bool IsFriend(const std::string& _friendID);
	const std::vector<std::string>& GetChatFriendList() const { return m_friendList; }
	const std::string& GetUserID() const { return m_loginID; }
};

