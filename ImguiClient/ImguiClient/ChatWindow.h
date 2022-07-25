#pragma once
#include "ImguiWindow.h"

class ChatWindow : public ImguiWindow
{
private: // member var
	

public: // default
	ChatWindow() = default;
	~ChatWindow() = default;

private:

public: // member Func
	virtual void UpdateWindow() override;
};

