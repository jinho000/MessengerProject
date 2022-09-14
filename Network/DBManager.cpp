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


bool DBManager::DoQueryAndSetResult(Query& _query)
{
	std::queue<std::string> queryResult;

	// ���� ����
	std::lock_guard<std::mutex> lock(m_dbMutex);

	if (mysql_query(m_mysqlLib, _query.GetQuery().c_str()) == 0) {

		// ����� �޾ƿ���
		MYSQL_RES* result = mysql_store_result(m_mysqlLib);
		if (result != nullptr)
		{
			// ����� ���ڵ带 �ϳ��� �޾ƿ�
			MYSQL_ROW row;
			while ((row = mysql_fetch_row(result)) != NULL)
			{
				for (int i = 0; i < _query.GetFieldCount(); ++i)
				{
					queryResult.push(row[i]);
				}
			}

			// ����� ó�� ����
			mysql_free_result(result);
		}

		// ���� ����� ����
		_query.SetQueryResult(std::move(queryResult));

		return true;
	}
	
	return false;
}
