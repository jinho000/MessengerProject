#pragma once
#include "Query.h"

class JoinQuery : public Query
{
public: // default
	JoinQuery(const std::string& _ID, const std::string& _PW);
	~JoinQuery();

public: // member Func
	virtual void ReadQuery() override;

};

