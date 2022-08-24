#include "ChatWindow.h"
#include "imgui.h"
#include "ImguiWindowManager.h"
#include "MainWindow.h"
#include "NetworkManager.h"

#include <PacketLibrary/ChattingPacket.h>
#include <PacketLibrary/ReadChattingPacket.h>

int ChatWindow::ID = 0;


ChatWindow::ChatWindow(const std::string& _friendID)
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
        if (chatMessage.bRead == false)
        {
            text += "(1)";
        }

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
        AddChatMessage(chatMessage);
        AddUnreadMessage(&m_messageList.back());

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

void ChatWindow::AddUnreadMessage(ChatMessage* _chatMessage)
{
    m_unreadMessage.push(_chatMessage);
}

void ChatWindow::AddRecvMessage(ChatMessage* _chatMessage)
{
    m_recvMessage.push_back(_chatMessage);
}

void ChatWindow::SendReadMessage()
{
    ReadChattingPacket packet(m_friendID, m_userID, m_recvMessage.size());
    NetworkManager::GetInst()->Send(&packet);

    m_recvMessage.clear();
}

void ChatWindow::ReadUnreadMessage(int _count)
{
    for (int i = 0; i < _count; ++i)
    {
        m_unreadMessage.front()->bRead = true;
        m_unreadMessage.pop();
    }
}

void ChatWindow::DispatchRecvChattingPacket(std::unique_ptr<PacketBase> _packet)
{
    std::unique_ptr<ChattingPacket> pPacket(static_cast<ChattingPacket*>(_packet.release()));
    ChatMessage& chatMessage = pPacket->GetChattingMessage();
    chatMessage.bRead = true;

    // 채팅창이 있는 경우
    MainWindow* pMainWindow = static_cast<MainWindow*>(ImguiWindowManager::GetInst()->GetImguiWindow(WINDOW_UI::MAIN));
    const std::vector<ChatWindow*>& chatWindowList = pMainWindow->GetChatWindowList();
    for (const auto& chatWindow : chatWindowList)
    {
        if (chatWindow->m_friendID == chatMessage.sendUserID)
        {
            chatWindow->AddChatMessage(chatMessage);
            chatWindow->AddRecvMessage(&chatWindow->m_messageList.back());
            
            if (chatWindow->IsActive() == true)
            {
                // 채팅창이 이미 열려있는경우
                chatWindow->SendReadMessage();
            }
            return;
        }
    }

    // 채팅창이 안만들어져 있는 경우 만들기
    ChatWindow* pChatWindow = new ChatWindow(chatMessage.sendUserID);
    pChatWindow->AddChatMessage(chatMessage);
    pChatWindow->AddRecvMessage(&pChatWindow->m_messageList.back());
    pChatWindow->SetActive(false);

    pMainWindow->AddChatWindow(pChatWindow);
}

void ChatWindow::DispatchReadMessagePacket(std::unique_ptr<PacketBase> _packet)
{
    std::unique_ptr<ReadChattingPacket> pPacket(static_cast<ReadChattingPacket*>(_packet.release()));

    MainWindow* pMainWindow = static_cast<MainWindow*>(ImguiWindowManager::GetInst()->GetImguiWindow(WINDOW_UI::MAIN));
    const std::vector<ChatWindow*>& chatWindowList = pMainWindow->GetChatWindowList();
    for (const auto& chatWindow : chatWindowList)
    {
        if (chatWindow->m_friendID == pPacket->GetRecvUserID())
        {
            chatWindow->ReadUnreadMessage(pPacket->GetReadCount());
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

void ChatWindow::AddChatMessage(const ChatMessage& _chatMessage)
{
    m_messageList.push_back(_chatMessage);
}

void ChatWindow::SetActive(bool _bActive)
{
    if (_bActive == true)
    {
        m_bActive = true;
        m_reclaimFocus = true;

        SendReadMessage();
    }
    else
    {
        m_bActive = false;
    }
}

std::string ChatWindow::GetFirstMessage()
{
    if (m_messageList.empty())
    {
        return "";
    }

    return m_messageList.back().message;
}

