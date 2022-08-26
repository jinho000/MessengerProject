#include "pch.h"
#include "QueryResult.h"

QueryResult::QueryResult(int _fieldCount)
	: m_fieldCount(_fieldCount)
{
}

QueryResult::~QueryResult()
{
}

void QueryResult::SetQueryResult(std::vector<std::string>&& _queryResult)
{
	m_queryResult = std::move(_queryResult);

	// 읽어온 쿼리결과를 가상함수를 통해 세팅
	ReadQuery();
}
