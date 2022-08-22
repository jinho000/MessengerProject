#pragma once
#include "ImguiWindow.h"
#include <string>

class ServerConnectModal : public ImguiWindow
{
private: // member var
	bool	m_bActive;
	bool	m_bConncetServer;

	std::string m_connectResult;

public: // default
	ServerConnectModal();
	~ServerConnectModal();

public: // member Func
	void UpdateWindow();
	
	void Active();
};

