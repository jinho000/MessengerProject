#pragma once
#include "ImguiWindow.h"
#include <string>

class JoinWindow : public ImguiWindow
{
private: // member var
	enum
	{
		BUFFER_SIZE = 64
	};

	char		m_IDBuffer[BUFFER_SIZE];
	char		m_PWBuffer[BUFFER_SIZE];
	bool		m_active;

	std::string m_checkResult;
	std::string m_JoinResult;

public: // default
	JoinWindow();
	~JoinWindow();

private:
	void ShowResultModal();

public: // member Func
	void UpdateWindow();

	void Active();
};

