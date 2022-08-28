#pragma once
#include "Query.h"

class LoginQuery : public Query
{
private: // member var
	int		m_userIDIndex;
	bool	m_bLoginSuccess;

public: // default
	LoginQuery(const std::string& _ID, const std::string& _PW);
	~LoginQuery();

public: // member Func
	virtual void ReadQuery();

	bool IsLoginSuccess() { return m_bLoginSuccess; }
	int GetUserIDIndex() { return m_userIDIndex; }
};

