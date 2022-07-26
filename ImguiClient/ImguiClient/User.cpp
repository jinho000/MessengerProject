#include "User.h"
#include "ChatFriend.h"

User::User(std::string _loginID, std::string _name, std::vector<ChatFriend*>& _chatFriendList)
	: m_loginID(_loginID)
	, m_name(_name)
	, m_chatFriendList(std::move(_chatFriendList))
{
}


User::~User()
{
	for (size_t i = 0; i < m_chatFriendList.size(); i++)
	{
		if (m_chatFriendList[i] != nullptr)
		{
			delete m_chatFriendList[i];
		}
	}
}

