#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>

using namespace std;

// lib
#include <mysql/mysql.h>
#pragma comment (lib, "libmysql.lib")


int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	cout << "Mysql Version: " << MYSQL_VERSION_ID << endl;
	cout << "Mysql Server Version: " << MYSQL_SERVER_VERSION << endl;
	cout << "Mysql Library Version: " << LIBMYSQL_VERSION << endl;

	MYSQL* mysql = nullptr;

	// 라이브러리 초기화
	mysql = mysql_init(mysql);
	if (nullptr == mysql)
	{
		cout << "init fail" << endl;
		return 0;
	}

	// DB에 연결
	MYSQL* Handle = mysql_real_connect(mysql, "127.0.0.1", "root", "1234", "userver2", 3306, nullptr, CLIENT_MULTI_RESULTS);
	if (mysql != Handle)
	{
		// 실패했을때 왜 실패했는지 알려줍니다.
		const char* Error = mysql_error(Handle);
		cout << "error: " << Error << endl;
		return 0;
	}

	// 쿼리 전송
	std::vector<string> vecResult;
	MYSQL_ROW row;
	char sql[1024] = { "SELECT ID FROM userver2.userinfo where Idx = 15;"};
	if (mysql_query(mysql, sql) == 0) {

		// 결과값 받아오기
		MYSQL_RES* result = mysql_store_result(mysql);

		// 결과값을 레코드하나씩 받아옴
		while ((row = mysql_fetch_row(result)) != NULL) 
		{
			MYSQL_FIELD* pFields = result->fields;
			vecResult.push_back(row[0]);
			vecResult.push_back(row[1]);
			vecResult.push_back(row[2]);
			vecResult.push_back(row[3]);
			vecResult.push_back(row[4]);
		}

		// 결과값 처리 해제
		mysql_free_result(result);
	}

	// 라이브러리 종료
	mysql_close(mysql);

	mysql_library_end();

	return 0;
}
