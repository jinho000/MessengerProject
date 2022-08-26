#pragma once
#include <mysql/mysql.h>
#include <string>
#include "QueryResult.h"

class DBManager 
{
private: // member var
	MYSQL* m_mysqlLib;

public: // default
	DBManager();
	~DBManager();

public:
	void DoQuery(const std::string& _query, QueryResult* _queryResult);

};

