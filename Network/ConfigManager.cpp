#include "pch.h"
#include "ConfigManager.h"

ConfigManager::ConfigManager()
	: m_serverIP()
	, m_serverPort()
	, m_maxConnection()
	, m_workerThreadCount()
{
}

ConfigManager::~ConfigManager()
{
}

void ConfigManager::LoadConfig()
{
	m_serverIP = "127.0.0.1";
	m_serverPort = 9900;
	m_maxConnection = 10;

	// CPU���� * 2 - 1��ŭ ������ �����
	// 1���� ���� ������
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	m_workerThreadCount = sysinfo.dwNumberOfProcessors * 2 - 1;


	//  "127.0.0.1", "root", "1234", "userver2", 3306,
	// DBServer ���� ����
	m_DBServerInfo.Host = "127.0.0.1";
	m_DBServerInfo.User = "root";
	m_DBServerInfo.Password = "1234";
	m_DBServerInfo.Schema = "userver2";
	m_DBServerInfo.Port = 3306;
}
