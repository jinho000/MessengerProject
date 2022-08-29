#pragma once
#include "Query.h"

class AddFriendQuery : public Query
{
private: // member var
	

public: // default
	AddFriendQuery(const std::string& _userID, const std::string& _friendID);
	~AddFriendQuery();

public: // member Func
	void ReadQuery();
};

