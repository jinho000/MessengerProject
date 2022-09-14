#pragma once
#include "Singleton.h"

struct DBServerInfo
{
	std::string Host;
	std::string User;
	std::string Password;
	std::string Schema;
	int			Port;
};

// 설정값을 읽어와 저장하는 클래스
// 저장한 설정값을 다른 클래스에서 가져다 사용
class ConfigManager : public Singleton<ConfigManager>
{
	friend class Singleton;

private: // member var
	std::string		m_serverIP;
	int				m_serverPort;
	int				m_maxConnection;
	int				m_LogicThreadCount;

	DBServerInfo	m_DBServerInfo;


private:
	ConfigManager();
	~ConfigManager();

public:
	// SetConfig
	void SetLogicThreadCount(UINT _count) { m_LogicThreadCount = _count; }
	
	void SetServerInfo(const std::string& _serverIP, int _serverPort, int _maxConnection);
	void SetDBServerInfo(DBServerInfo _dbServerInfo);
	

	// Get Config
	const std::string& GetServerIP() { return m_serverIP; }
	int GetServerPort() { return m_serverPort; }
	int GetMaxConnection() { return m_maxConnection; }
	int GetLogicThreadCount() { return m_LogicThreadCount; }
	const DBServerInfo& GetDBServerInfo() { return m_DBServerInfo; }
};

