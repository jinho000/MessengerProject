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
	MAIN_UI_TYPE m_mainUIType;
	User*		 m_pLoginUser;

	std::vector<ChatWindow*> m_chatWindowList;


public: // default
	MainWindow();
	~MainWindow();

private:
	void CreateChatWindow(int _count);


public: // member Func
	virtual void UpdateWindow() override;

	void SetLoginUser(User* _pLoginUser);

};

