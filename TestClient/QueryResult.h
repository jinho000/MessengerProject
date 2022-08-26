#pragma once
#include <vector>
#include <string>
#include <mysql/mysql.h>

class QueryResult
{
private: // member var
	std::vector<std::string>	m_queryResult;
	int							m_fieldCount;

public: // default
	QueryResult(int _fieldCount);
	~QueryResult();

	QueryResult(const QueryResult& _other) = delete;
	QueryResult(QueryResult&& _other) = delete;
	QueryResult& operator=(const QueryResult& _other) = delete;
	QueryResult& operator=(const QueryResult&& _other) = delete;

public: // member Func
	int GetFieldCount() { return m_fieldCount; }
	void SetQueryResult(std::vector<std::string>&& _queryResult);

	virtual void ReadQuery() = 0;
};

