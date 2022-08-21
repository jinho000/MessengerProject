#include "User.h"


User::User(std::string _loginID, std::vector<std::string>& _friendList)
	: m_loginID(_loginID)
	, m_friendList(std::move(_friendList))
{
}


User::~User()
{

}

