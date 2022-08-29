#pragma once
#include "Query.h"

class FriendListQuery : public Query
{
private: // member var
	std::vector<std::string> m_friendList;

public: // default
	FriendListQuery(const std::string& _userID);
	~FriendListQuery();

public: // member Func
	virtual void ReadQuery();

	std::vector<std::string>&& GetFriendList() { return std::move(m_friendList); }
};

