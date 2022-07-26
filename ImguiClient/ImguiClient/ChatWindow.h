#pragma once
#include "ImguiWindow.h"
#include "ChatFriend.h"
#include <vector>
#include <string>

class ChatWindow : public ImguiWindow
{
private: // member var
	std::string	m_ID;
	ChatFriend*	m_pChatFriend;
	bool		m_bActive;

public: // default
	ChatWindow();
	~ChatWindow() = default;


public: // member Func
	virtual void UpdateWindow() override;

	void SetChatFriend(ChatFriend* _pChatFriend);
};

