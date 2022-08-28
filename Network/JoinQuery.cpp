#include "pch.h"
#include "JoinQuery.h"


JoinQuery::JoinQuery(const std::string& _ID, const std::string& _PW)
	: Query(std::string("insert into userinfo (ID, PW) values (\"" + _ID +"\", \"" + _PW + "\");"), 0)
{
}

JoinQuery::~JoinQuery()
{
}

void JoinQuery::ReadQuery()
{
}
