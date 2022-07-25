#pragma once
#include <vector>
#include <string>


class User
{
private: // member var
	std::string			m_ID;
	std::string			m_name;
	
	std::vector<User*>	m_friendList;
	

public: // default
	User();
	~User();

	User(const User& _other) = delete;
	User(User&& _other) = delete;
	User& operator=(const User& _other) = delete;
	User& operator=(const User&& _other) = delete;

public: // member Func

};

