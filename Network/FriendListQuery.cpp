#include "pch.h"
#include "FriendListQuery.h"

FriendListQuery::FriendListQuery(const std::string& _userID)
	: Query(std::string("select friendList.friendID from userinfo inner join friendList on userinfo.ID = ") + _userID + ";", 1)
{
}

FriendListQuery::~FriendListQuery()
{
}

void FriendListQuery::ReadQuery()
{
	while (m_queryResult.empty() != true)
	{
		m_friendList.push_back(std::move(m_queryResult.front()));
		m_queryResult.pop();
	}
}
