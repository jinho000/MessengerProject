#pragma once
#include "ImguiWindow.h"
#include "imgui.h"
#include <queue>
#include <vector>
#include <string>

#include <PacketLibrary/struct.h>
#include <PacketLibrary/PacketBase.h>

class ChatWindow : public ImguiWindow
{
private: // member var
	static int ID;

	std::string					m_windowID;
	std::string					m_friendID;
	std::string					m_userID;
	std::vector<ChatMessage*>	m_messageList;

	std::vector<ChatMessage*>	m_recvMessage;
	std::queue<ChatMessage*>	m_unreadMessage;

	bool						m_bActive;

	bool						m_ScrollToBottom;
	bool						m_reclaimFocus;
	
	ImVec2						m_initialSize;

private:
	void ShowChattingMessage();
	void ShowInputMessage();

	void AddUnreadMessage(ChatMessage* _chatMessage);
	void AddRecvMessage(ChatMessage* _chatMessage);

	void SendReadMessage();
	void ReadUnreadMessage(int _count);

public: // default
	ChatWindow(const std::string& _friendID);
	~ChatWindow() = default;

public:
	static void DispatchRecvChattingPacket(std::unique_ptr<PacketBase> _packet);
	static void DispatchRecvChatting(const ChatMessage& _pChatMessage);
	static void DispatchReadMessagePacket(std::unique_ptr<PacketBase> _packet);

public: // member Func
	virtual void UpdateWindow() override;


	void AddChatMessage(ChatMessage* _chatMessage);
	void SetActive(bool _bActive);
	bool IsActive() { return m_bActive; }

	const std::string& GetFriendID() const { return m_friendID; }
	std::string GetFirstMessage();

};

