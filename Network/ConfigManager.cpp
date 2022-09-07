#include "pch.h"
#include "ConfigManager.h"

ConfigManager::ConfigManager()
	: m_serverIP()
	, m_serverPort()
	, m_maxConnection()
	, m_workerThreadCount()
{
	// CPU개수 * 2 - 1만큼 스레드 만들기
	// 1개는 메인 스레드
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	m_workerThreadCount = sysinfo.dwNumberOfProcessors * 2 - 1;
}

ConfigManager::~ConfigManager()
{
}

void ConfigManager::SetServerInfo(const std::string& _serverIP, int _serverPort, int _maxConnection)
{
	m_serverIP = _serverIP;
	m_serverPort = _serverPort;
	m_maxConnection = _maxConnection;

	//m_serverIP = "127.0.0.1";
	//m_serverPort = 9900;
	//m_maxConnection = 10;
}

void ConfigManager::SetDBServerInfo(DBServerInfo _dbServerInfo)
{
	m_DBServerInfo = _dbServerInfo;

	//m_DBServerInfo.Host = "127.0.0.1";
	//m_DBServerInfo.User = "root";
	//m_DBServerInfo.Password = "1234";
	//m_DBServerInfo.Schema = "messengerdb";
	//m_DBServerInfo.Port = 3306;
}
