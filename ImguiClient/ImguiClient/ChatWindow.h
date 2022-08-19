#pragma once
#include "ImguiWindow.h"
#include <vector>
#include <string>

#include <PacketLibrary/struct.h>

class ChatWindow : public ImguiWindow
{
private: // member var
	std::string					m_friendID;
	std::vector<ChatMessage>	m_messageList;
	bool						m_bActive;

public: // default
	ChatWindow();
	~ChatWindow() = default;


public: // member Func
	virtual void UpdateWindow() override;

};

