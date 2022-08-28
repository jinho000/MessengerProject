#include "User.h"


User::User(std::string _loginID, std::vector<std::string>& _friendList)
	: m_loginID(_loginID)
	, m_friendList(std::move(_friendList))
{
}


User::~User()
{

}

bool User::IsFriend(const std::string& _friendID)
{
	auto iter = std::find(m_friendList.begin(), m_friendList.end(), _friendID);
	
	return iter != m_friendList.end();
}

