#include "pch.h"
#include "FriendListQuery.h"

FriendListQuery::FriendListQuery(int _userIDIndex)
	: Query(std::string("select friendList.friendID from userinfo inner join friendList on userinfo.idx = ") + std::to_string(_userIDIndex) + ";", 1)
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
