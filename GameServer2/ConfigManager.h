#pragma once
#include "Singleton.h"

// 설정값을 읽어와 저장하는 클래스
// 저장한 설정값을 다른 클래스에서 가져다 사용
class ConfigManager : public Singleton<ConfigManager>
{
	friend class Singleton;

private: // member var
	std::string m_serverIP;
	int			m_serverPort;

private:
	ConfigManager();
	~ConfigManager();

public:
	void LoadConfig();


	// Get Config
	const std::string& GetServerIP() { return m_serverIP; }
	int GetServerPort() { return m_serverPort; }

};

