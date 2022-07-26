#include "ChatWindow.h"
#include "imgui.h"


ChatWindow::ChatWindow()
	: m_ID()
	, m_pChatFriend(nullptr)
	, m_bActive(false)
{
}

void ChatWindow::UpdateWindow()
{
	if (m_bActive == false)
		m_pChatFriend = nullptr;

	if (m_pChatFriend == nullptr)
		return;

	ImGui::Begin(m_ID.c_str(), &m_bActive);



	ImGui::End();
}

void ChatWindow::SetChatFriend(ChatFriend* _pChatFriend)
{
	m_pChatFriend = _pChatFriend;
	m_bActive = true;
	m_ID = std::string("ChatWindow ") + std::to_string(reinterpret_cast<unsigned int>(m_pChatFriend));
}
