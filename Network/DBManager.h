#pragma once
#include "Singleton.h"
#include "QueryResult.h"

class DBManager : public Singleton<DBManager>
{
	friend class Singleton;
private: // member var
	MYSQL* m_mysqlLib;

private: // default
	DBManager();
	~DBManager();

public:
	void DoQueryAndSetResult(const std::string& _query, QueryResult& _queryResult);
};

