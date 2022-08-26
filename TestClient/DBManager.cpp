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

	// 서버 시작시 디비에 연결
	MYSQL* Handle = mysql_real_connect(m_mysqlLib, "127.0.0.1", "root", "1234", "userver2", 3306, nullptr, CLIENT_MULTI_RESULTS);
	if (m_mysqlLib != Handle)
	{
		// 실패했을때 왜 실패했는지 알려줍니다.
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

	// 쿼리 전송
	if (mysql_query(m_mysqlLib, _query.c_str()) == 0) {

		// 결과값 받아오기
		MYSQL_RES* result = mysql_store_result(m_mysqlLib);
		MYSQL_ROW row;

		// 결과값 레코드를 하나씩 받아옴
		while ((row = mysql_fetch_row(result)) != NULL)
		{
			for (int i = 0; i < _queryResult->GetFieldCount(); ++i)
			{
				queryResult.push_back(row[i]);
			}
		}

		// 결과값 처리 해제
		mysql_free_result(result);
	}

	_queryResult->SetQueryResult(std::move(queryResult));
}
