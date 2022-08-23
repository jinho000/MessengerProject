#include "ChatWindow.h"
#include "imgui.h"
#include "ImguiWindowManager.h"
#include "MainWindow.h"
#include "NetworkManager.h"

#include <PacketLibrary/ChattingPacket.h>

int ChatWindow::ID = 0;


ChatWindow::ChatWindow(const std::string& _friendID)
    : ChatWindow(_friendID, "")
{
}

ChatWindow::ChatWindow(const std::string& _friendID, const std::string& _message)
    : m_windowID(std::to_string(ID) + "_")
    , m_friendID(_friendID)
    , m_bActive(true)
    , m_ScrollToBottom(false)
    , m_reclaimFocus(true)
    , m_initialSize(ImVec2(500, 400))
{
    m_windowID += _friendID;
    MainWindow* pMainWindow = static_cast<MainWindow*>(ImguiWindowManager::GetInst()->GetImguiWindow(WINDOW_UI::MAIN));
    m_userID = pMainWindow->GetUser()->GetUserID();

    if (_message.empty() == false)
    {
        ChatMessage message;
        message.sendUserID = _friendID;
        message.recvUserID = m_userID;
        message.message = _message;
        m_messageList.push_back(message);
    }
}


void ChatWindow::ShowChattingMessage()
{
    // ��ũ�ѹ� �����
    const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);

    // ä�� ���
    for (const ChatMessage& chatMessage : m_messageList)
    {
        std::string text(chatMessage.sendUserID + ": " + chatMessage.message);
        ImGui::Text(text.c_str());
    }

    // ��ũ�� �ڵ� ������
    if (m_ScrollToBottom || (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
        ImGui::SetScrollHereY(1.0f);
    m_ScrollToBottom = false;

    ImGui::EndChild();
}

void ChatWindow::ShowInputMessage()
{
    // ä�� �Է�
    char buff[255] = {};
    if (ImGui::InputText("##ChatInput", buff, IM_ARRAYSIZE(buff), ImGuiInputTextFlags_EnterReturnsTrue))
    {
        // ä��â�� ���
        ChatMessage chatMessage = { m_userID, m_friendID, buff };
        m_messageList.push_back(chatMessage);

        // ������ ����
        ChattingPacket packet(chatMessage);
        NetworkManager::GetInst()->Send(&packet);

        m_ScrollToBottom = true;
        m_reclaimFocus = true;
    }

    // �Է�â ��Ŀ��
    ImGui::SetItemDefaultFocus();
    if (m_reclaimFocus)
    {
        ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget
        m_reclaimFocus = false;
    }
}

void ChatWindow::DispatchRecvChattingPacket(std::unique_ptr<PacketBase> _packet)
{
    std::unique_ptr<ChattingPacket> pPacket(static_cast<ChattingPacket*>(_packet.release()));
    
    
    // ä��â�� �ִ� ���
    MainWindow* pMainWindow = static_cast<MainWindow*>(ImguiWindowManager::GetInst()->GetImguiWindow(WINDOW_UI::MAIN));
    const std::vector<ChatWindow*>& chatWindowList = pMainWindow->GetChatWindowList();
    for (const auto& chatWindow : chatWindowList)
    {
        if (chatWindow->m_friendID == pPacket->GetChattingMessage().sendUserID)
        {
            chatWindow->m_messageList.push_back(pPacket->GetChattingMessage());
            return;
        }
    }

    // ä��â�� �ȸ������ �ִ� ��� �����
    pMainWindow->CreateChatWindow(pPacket->GetChattingMessage().sendUserID, pPacket->GetChattingMessage().message);
}

void ChatWindow::UpdateWindow()
{
	if (m_bActive == false)
		return;

	ImGui::SetNextWindowSize(m_initialSize, ImGuiCond_FirstUseEver);
	ImGui::Begin(m_windowID.c_str(), &m_bActive);

    ShowChattingMessage();

    ImGui::Separator();

    ShowInputMessage();

	ImGui::End();
}

void ChatWindow::Active()
{
    m_bActive = true;
    m_reclaimFocus = true;
}

std::string ChatWindow::GetFirstMessage()
{
    if (m_messageList.empty())
    {
        return "";
    }

    return m_messageList.back().message;
}

