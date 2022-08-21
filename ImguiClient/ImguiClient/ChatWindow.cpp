#include "ChatWindow.h"
#include "imgui.h"
#include "ImguiWindowManager.h"
#include "MainWindow.h"
#include "NetworkManager.h"

#include <PacketLibrary/SendChattingPacket.h>
#include <PacketLibrary/RecvChattingPacket.h>

int ChatWindow::ID = 0;


ChatWindow::ChatWindow(const std::string& _friendID)
	: m_windowID(std::to_string(ID) + "_")
	, m_friendID(_friendID)
    //, m_chatMessage()
	, m_bActive(true)
    , m_ScrollToBottom(false)
    , m_reclaimFocus(true)
    , m_initialSize(ImVec2(500, 400))
{
	m_windowID += _friendID;
    MainWindow* pMainWindow = static_cast<MainWindow*>(ImguiWindowManager::GetInst()->GetImguiWindow(WINDOW_UI::MAIN));
    m_userID = pMainWindow->GetUser()->GetUserID();
}


void ChatWindow::ShowChattingMessage()
{
    // 스크롤바 만들기
    const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);

    // 채팅 출력
    for (const ChatMessage& chatMessage : m_messageList)
    {
        std::string text(chatMessage.ID + ": " + chatMessage.message);
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
        m_messageList.push_back({ m_userID, buff });

        // 서버로 전송
        SendChattingPacket packet(m_userID, m_friendID, buff);

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
    std::unique_ptr<RecvChattingPacket> pPacket(static_cast<RecvChattingPacket*>(_packet.release()));
    MainWindow* pMainWindow = static_cast<MainWindow*>(ImguiWindowManager::GetInst()->GetImguiWindow(WINDOW_UI::MAIN));
    const std::vector<ChatWindow*>& chatWindowList = pMainWindow->GetChatWindowList();
    
    for (const auto& chatWindow : chatWindowList)
    {
        if (chatWindow->m_friendID == pPacket->GetSendUserID())
        {
            chatWindow->m_messageList.push_back({ pPacket->GetSendUserID(), pPacket->GetChatMessage()});
            break;
        }
    }
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

