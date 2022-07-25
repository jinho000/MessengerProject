#pragma once
#include "ImguiWindow.h"
#include "User.h"
#include "ChatWindow.h"

#include <vector>

enum class MAIN_UI_TYPE
{
	FRIEND_LIST,
	CHATWINDOW_LIST,
	END
};

class MainWindow : public ImguiWindow
{
private: // member var
	MAIN_UI_TYPE				m_mainUIType;
	User*						m_loginUser;
	std::vector<ChatWindow*>	m_chatWindow;

public: // default
	MainWindow();
	~MainWindow();

public: // member Func
	virtual void UpdateWindow() override;

	void SetLoginUser(User* _pLoginUser) { m_loginUser = _pLoginUser; }

};

