#include "pch.h"
#include "AddFriendQuery.h"


AddFriendQuery::AddFriendQuery(const std::string& _userID, const std::string& _friendID)
	: Query(std::string("insert into friendList (userID, friendID) values (\"") + _userID + "\", \"" + _friendID + "\");", 0)
{
}

AddFriendQuery::~AddFriendQuery()
{
}

void AddFriendQuery::ReadQuery()
{
}
