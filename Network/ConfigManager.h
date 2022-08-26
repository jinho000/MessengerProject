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

// �������� �о�� �����ϴ� Ŭ����
// ������ �������� �ٸ� Ŭ�������� ������ ���
class ConfigManager : public Singleton<ConfigManager>
{
	friend class Singleton;

private: // member var
	std::string		m_serverIP;
	int				m_serverPort;
	int				m_maxConnection;
	int				m_workerThreadCount;

	DBServerInfo	m_DBServerInfo;


private:
	ConfigManager();
	~ConfigManager();

public:
	void LoadConfig();


	// Get Config
	const std::string& GetServerIP() { return m_serverIP; }
	int GetServerPort() { return m_serverPort; }
	int GetMaxConnection() { return m_maxConnection; }
	int GetWorkerThreadCount() { return m_workerThreadCount; }
	const DBServerInfo& GetDBServerInfo() { return m_DBServerInfo; }
};

