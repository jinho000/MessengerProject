#pragma once
#include "Query.h"

// select idx from userinfo where id = "aaa";
class FindIDQuery : public Query
{
private: // member var
	static std::string gFindIDQuery;

	std::string	m_userID;
	bool		m_bFindSuccess;

public: // default
	FindIDQuery(const std::string& _findID);
	~FindIDQuery();

public: // member Func
	virtual void ReadQuery() override;

	bool IsFindSuccess() { return m_bFindSuccess; }
	const std::string& GetID() { return m_userID; }
};

