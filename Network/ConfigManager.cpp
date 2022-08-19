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
	m_maxConnection = 1;

	// CPU���� * 2 - 1��ŭ ������ �����
	// 1���� ���� ������
	//SYSTEM_INFO sysinfo;
	//GetSystemInfo(&sysinfo);

	//m_workerThreadCount = sysinfo.dwNumberOfProcessors * 2 - 1;

	m_workerThreadCount = 1;
}
