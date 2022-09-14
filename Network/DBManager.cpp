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

	// 서버 시작시 디비에 연결
	
	// 설정파일에서 디비 정보 세팅하기
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
		// 디비 연결 실패
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

	// 쿼리 전송
	std::lock_guard<std::mutex> lock(m_dbMutex);

	if (mysql_query(m_mysqlLib, _query.GetQuery().c_str()) == 0) {

		// 결과값 받아오기
		MYSQL_RES* result = mysql_store_result(m_mysqlLib);
		if (result != nullptr)
		{
			// 결과값 레코드를 하나씩 받아옴
			MYSQL_ROW row;
			while ((row = mysql_fetch_row(result)) != NULL)
			{
				for (int i = 0; i < _query.GetFieldCount(); ++i)
				{
					queryResult.push(row[i]);
				}
			}

			// 결과값 처리 해제
			mysql_free_result(result);
		}

		// 쿼리 결과값 세팅
		_query.SetQueryResult(std::move(queryResult));

		return true;
	}
	
	return false;
}
