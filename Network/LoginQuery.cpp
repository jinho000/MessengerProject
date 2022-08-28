#include "pch.h"
#include "LoginQuery.h"

LoginQuery::LoginQuery(const std::string& _ID, const std::string& _PW)
	: Query(std::string("select idx from userinfo where id = \"") 
		+ _ID + "\" AND pw = \"" 
		+ _PW + "\";", 1)
	, m_userIDIndex(-1)
	, m_bLoginSuccess(false)
{
}

LoginQuery::~LoginQuery()
{
}

void LoginQuery::ReadQuery()
{
	if (m_queryResult.empty() != true)
	{
		m_userIDIndex = std::stoi(m_queryResult.front());
		m_bLoginSuccess = true;
	}
}
