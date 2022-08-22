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

	// CPU개수 * 2 - 1만큼 스레드 만들기
	// 1개는 메인 스레드
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);

	m_workerThreadCount = sysinfo.dwNumberOfProcessors * 2 - 1;

	//m_workerThreadCount = 1;
}
