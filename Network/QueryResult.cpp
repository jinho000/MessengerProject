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

	// �о�� ��������� �����Լ��� ���� ����
	ReadQuery();
}
