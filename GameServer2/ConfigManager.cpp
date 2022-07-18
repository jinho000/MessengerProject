#include "pch.h"
#include "ConfigManager.h"

ConfigManager::ConfigManager()
	: m_serverIP()
	, m_serverPort()
{
}

ConfigManager::~ConfigManager()
{
}

void ConfigManager::LoadConfig()
{
	m_serverIP = "127.0.0.1";
	m_serverPort = 9900;

}
