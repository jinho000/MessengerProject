#pragma once
#include "Singleton.h"
#include "Query.h"

class DBManager : public Singleton<DBManager>
{
	friend class Singleton;
private: // member var
	MYSQL* m_mysqlLib;

private: // default
	DBManager();
	~DBManager();

public:
	bool DoQueryAndSetResult(Query& _query);
};

