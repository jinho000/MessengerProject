#pragma once
#include "ImguiWindow.h"
#include <string>

class LoginWindow : public ImguiWindow
{
private: // member var
	enum
	{
		BUFFER_SIZE = 64
	};

	char m_IDBuffer[BUFFER_SIZE];
	char m_PWBuffer[BUFFER_SIZE];


public: // default
	LoginWindow();
	~LoginWindow() = default;


public: // member Func
	virtual void UpdateWindow() override;
};

