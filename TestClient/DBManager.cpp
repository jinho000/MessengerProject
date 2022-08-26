#include "DBManager.h"
#include <vector>

DBManager::DBManager()
	: m_mysqlLib(nullptr)
{
	m_mysqlLib = mysql_init(m_mysqlLib);
	if (m_mysqlLib == nullptr)
	{
		//cout << "mysql init error" << endl;
	}

	// ���� ���۽� ��� ����
	MYSQL* Handle = mysql_real_connect(m_mysqlLib, "127.0.0.1", "root", "1234", "userver2", 3306, nullptr, CLIENT_MULTI_RESULTS);
	if (m_mysqlLib != Handle)
	{
		// ���������� �� �����ߴ��� �˷��ݴϴ�.
		const char* Error = mysql_error(Handle);
		//cout << "error: " << Error << endl;
	}
}

DBManager::~DBManager()
{
	if (m_mysqlLib == nullptr)
		return;

	mysql_close(m_mysqlLib);
}

void DBManager::DoQuery(const std::string& _query, QueryResult* _queryResult)
{
	std::vector<std::string> queryResult;

	// ���� ����
	if (mysql_query(m_mysqlLib, _query.c_str()) == 0) {

		// ����� �޾ƿ���
		MYSQL_RES* result = mysql_store_result(m_mysqlLib);
		MYSQL_ROW row;

		// ����� ���ڵ带 �ϳ��� �޾ƿ�
		while ((row = mysql_fetch_row(result)) != NULL)
		{
			for (int i = 0; i < _queryResult->GetFieldCount(); ++i)
			{
				queryResult.push_back(row[i]);
			}
		}

		// ����� ó�� ����
		mysql_free_result(result);
	}

	_queryResult->SetQueryResult(std::move(queryResult));
}
