#pragma once
#include "ImguiWindow.h"
#include "imgui.h"
#include <vector>
#include <string>

#include <PacketLibrary/struct.h>

class ChatWindow : public ImguiWindow
{
private: // member var
	static int ID;

	std::string					m_windowID;
	std::string					m_friendID;
	std::string					m_userID;
	std::vector<ChatMessage>	m_messageList;
	bool						m_bActive;

	bool						m_ScrollToBottom;
	bool						m_reclaimFocus;
	
	ImVec2						m_initialSize;

private:
	void ShowChattingMessage();
	void ShowInputMessage();
	
public: // default
	ChatWindow(const std::string& _friendID);
	~ChatWindow() = default;


public: // member Func
	virtual void UpdateWindow() override;

	void Active() 
	{ 
		m_bActive = true;
		m_reclaimFocus = true;
	}
	void SetScrollToBottom() { m_ScrollToBottom = true; };
};

