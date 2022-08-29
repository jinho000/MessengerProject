#include "pch.h"
#include "FindIDQuery.h"

//std::string FindIDQuery::gFindIDQuery = "select idx from userinfo where id = ";

FindIDQuery::FindIDQuery(const std::string& _findID)
	//: Query(QUERY_TYPE::FIND_ID, 1)
	: Query(std::string("select userinfo.ID from userinfo where id = \"" + _findID + "\";"), 1)
	, m_bFindSuccess(false)
{
}

FindIDQuery::~FindIDQuery()
{
}

void FindIDQuery::ReadQuery()
{
	if (m_queryResult.empty() != true)
	{
		m_userID = m_queryResult.front();
		m_bFindSuccess = true;
	}
}
