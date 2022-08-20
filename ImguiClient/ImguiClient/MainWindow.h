#pragma once
#include "ImguiWindow.h"
#include "User.h"
#include "ChatWindow.h"

#include <vector>
#include <unordered_map>

enum class MAIN_UI_TYPE
{
	FRIEND_LIST,
	CHATWINDOW_LIST,
	END
};

class MainWindow : public ImguiWindow
{
private: // member var
	User*		 m_pLoginUser;

	std::unordered_map<std::string, ChatWindow*>	m_charWindowMap;
	std::vector<ChatWindow*>						m_chatWindowList;


public: // default
	MainWindow();
	~MainWindow();

private:
	void CreateChatWindow(const std::string& _friend);
	

public: // member Func
	virtual void UpdateWindow() override;

	void SetLoginUser(User* _pLoginUser);
	const User* GetUser() { return m_pLoginUser; }
};

