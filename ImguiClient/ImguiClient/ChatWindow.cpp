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

	const std::vector<ChatMessage>& messageList = m_pChatFriend->GetMessageList();
	for (size_t i = 0; i < messageList.size(); i++)
	{
		ImGui::Text("%s: %s", messageList[i].ID.c_str(), messageList[i].message.c_str());
	}

	ImGui::Separator();

	ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue;
	char buff[255] = {};
	if (ImGui::InputText(" ##ss", buff, 255, input_text_flags))
	{
		int a = 0;
	}

	ImGui::End();
}

void ChatWindow::SetChatFriend(ChatFriend* _pChatFriend)
{
	m_pChatFriend = _pChatFriend;
	m_bActive = true;
	m_ID = std::string("ChatWindow ") + std::to_string(reinterpret_cast<unsigned int>(m_pChatFriend));
}
