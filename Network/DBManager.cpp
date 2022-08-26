#include "pch.h"
#include "DBManager.h"
#include "ConfigManager.h"

DBManager::DBManager()
	: m_mysqlLib(nullptr)
{
	m_mysqlLib = mysql_init(m_mysqlLib);
	if (m_mysqlLib == nullptr)
	{
		//cout << "mysql init error" << endl;
		assert(nullptr);
	}

	// ���� ���۽� ��� ����
	
	// �������Ͽ��� ��� ���� �����ϱ�
	const DBServerInfo& info = ConfigManager::GetInst()->GetDBServerInfo();
	MYSQL* Handle = mysql_real_connect(m_mysqlLib
		, info.Host.c_str()
		, info.User.c_str()
		, info.Password.c_str()
		, info.Schema.c_str()
		, info.Port
		, nullptr
		, CLIENT_MULTI_RESULTS
	);

	if (m_mysqlLib != Handle)
	{
		// ��� ���� ����
		const char* Error = mysql_error(Handle);
		assert(nullptr);
	}
}

DBManager::~DBManager()
{
	if (m_mysqlLib == nullptr)
		return;

	mysql_close(m_mysqlLib);
}

void DBManager::DoQueryAndSetResult(const std::string& _query, QueryResult& _queryResult)
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
			for (int i = 0; i < _queryResult.GetFieldCount(); ++i)
			{
				queryResult.push_back(row[i]);
			}
		}

		// ����� ó�� ����
		mysql_free_result(result);
	}

	// ���� ����� ����
	_queryResult.SetQueryResult(std::move(queryResult));
}
