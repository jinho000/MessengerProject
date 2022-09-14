#include "pch.h"
#include "ConfigManager.h"

ConfigManager::ConfigManager()
	: m_serverIP()
	, m_serverPort()
	, m_maxConnection()
	, m_LogicThreadCount()
{
}

ConfigManager::~ConfigManager()
{
}

void ConfigManager::SetServerInfo(const std::string& _serverIP, int _serverPort, int _maxConnection)
{
	m_serverIP = _serverIP;
	m_serverPort = _serverPort;
	m_maxConnection = _maxConnection;
}

void ConfigManager::SetDBServerInfo(DBServerInfo _dbServerInfo)
{
	m_DBServerInfo = _dbServerInfo;
}
