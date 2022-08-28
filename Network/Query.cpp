#include "pch.h"
#include "Query.h"

//std::string Query::gQueryString[QUERY_COUNT] =
//{
//	"select idx from userinfo where id = ",
//	"insert into userinfo (ID, PW) values ("ID5", "ww");"
//};

Query::Query(QUERY_TYPE _queryType, int _fieldCount)
	: m_query(MakeQueryString(_queryType))
	, m_fieldCount(_fieldCount)
{
}

Query::Query(const std::string& _queryString, int _fieldCount)
	: m_query(_queryString)
	, m_fieldCount(_fieldCount)
{
}

Query::~Query()
{
}

std::string Query::MakeQueryString(QUERY_TYPE _queryType)
{
	// ������
	return std::string();
}

void Query::SetQueryResult(std::queue<std::string>&& _queryResult)
{
	m_queryResult = std::move(_queryResult);

	// �о�� ��������� �����Լ��� ���� ����
	ReadQuery();
}
