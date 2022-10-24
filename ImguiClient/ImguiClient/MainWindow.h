#pragma once
#include <PacketLibrary/PacketBase.h>
#include <vector>
#include <unordered_map>

#include "ImguiWindow.h"
#include "User.h"
#include "ChatWindow.h"


enum class MAIN_UI_TYPE
{
	FRIEND_LIST,
	CHATWINDOW_LIST,
	END
};

class MainWindow : public ImguiWindow
{
private: // member var
	User*						m_pLoginUser;

	std::vector<ChatWindow*>	m_chatWindowList;

	bool		m_bAddFriendPopup;
	std::string m_addFriendResult;
	char		m_addFriendID[255];

	bool		m_bPopupInputFocus;
	bool		m_bLogout;


public: // default
	MainWindow();
	~MainWindow();

public:
	static void DispatchAddFriendResultPacket(std::unique_ptr<PacketBase> _packet);
	static void DispatchClientExitPacket(std::unique_ptr<PacketBase> _packet);

private:
	void ShowAddFriendPopup();
	void UpdateAddFriendPopup();

	void ShowUserInfoOrMessageList();


	void LogoutUser();
	void DestroyChatWindow();

public: // member Func
	virtual void UpdateWindow() override;

	void AddChatWindow(ChatWindow* _chatWindow);
	void CreateChatWindow(const std::string& _friend);
	void MoveLoginWindow();

	void SetLoginUser(User* _pLoginUser);
	const User* GetUser() { return m_pLoginUser; }
	const std::vector<ChatWindow*>& GetChatWindowList() { return m_chatWindowList; }

};

