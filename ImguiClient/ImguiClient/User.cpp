#include "User.h"
#include "ChatFriend.h"

User::User(std::string _loginID, std::string _name, std::vector<std::string>& _friendList)
	: m_loginID(_loginID)
	, m_name(_name)
	, m_friendList(std::move(_friendList))
{
}


User::~User()
{

}

