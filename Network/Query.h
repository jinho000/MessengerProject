#pragma once

enum class QUERY_TYPE
{
	FIND_ID,
	JOIN,
	END
};

class Query
{
protected: // member var
	std::string					m_query;

	std::queue<std::string>		m_queryResult;
	int							m_fieldCount;

public: // default
	Query(QUERY_TYPE _queryType, int _fieldCount);
	Query(const std::string& _queryString, int _fieldCount);
	~Query();

	Query(const Query& _other) = delete;
	Query(Query&& _other) = delete;
	Query& operator=(const Query& _other) = delete;
	Query& operator=(const Query&& _other) = delete;

protected:
	//template<typename T, t
	std::string MakeQueryString(QUERY_TYPE _queryType);

public: // member Func
	const std::string& GetQuery() { return m_query; }
	int GetFieldCount() { return m_fieldCount; }

	void SetQueryResult(std::queue<std::string>&& _queryResult);

	virtual void ReadQuery() = 0;

};

