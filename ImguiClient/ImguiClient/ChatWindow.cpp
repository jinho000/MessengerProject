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
    // 스크롤바 만들기
    const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);

    // 채팅 출력
    for (const ChatMessage& chatMessage : m_messageList)
    {
        std::string text(chatMessage.sendUserID + ": " + chatMessage.message);
        ImGui::Text(text.c_str());
    }

    // 스크롤 자동 내리기
    if (m_ScrollToBottom || (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
        ImGui::SetScrollHereY(1.0f);
    m_ScrollToBottom = false;

    ImGui::EndChild();
}

void ChatWindow::ShowInputMessage()
{
    // 채팅 입력
    char buff[255] = {};
    if (ImGui::InputText("##ChatInput", buff, IM_ARRAYSIZE(buff), ImGuiInputTextFlags_EnterReturnsTrue))
    {
        // 채팅창에 출력
        ChatMessage chatMessage = { m_userID, m_friendID, buff };
        m_messageList.push_back(chatMessage);

        // 서버로 전송
        ChattingPacket packet(chatMessage);
        NetworkManager::GetInst()->Send(&packet);

        m_ScrollToBottom = true;
        m_reclaimFocus = true;
    }

    // 입력창 포커싱
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
    
    
    // 채팅창이 있는 경우
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

    // 채팅창이 안만들어져 있는 경우 만들기
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

