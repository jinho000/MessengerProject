#include "LoginWindow.h"
#include "imgui.h"
#include "ImguiWindowManager.h"
#include "User.h"
#include "MainWindow.h"
#include "NetworkManager.h"

#include <PacketLibrary/LoginPacket.h>
#include <PacketLibrary/LoginResultPacket.h>

LoginWindow::LoginWindow()
	: m_IDBuffer()
	, m_PWBuffer()
	, m_joinModal()
{
}

void LoginWindow::DispatchLoginResultPacket(std::unique_ptr<PacketBase> _packet)
{
	LoginWindow* pLoginWindow = static_cast<LoginWindow*>(ImguiWindowManager::GetInst()->GetImguiWindow(WINDOW_UI::LOGIN));
	
	std::unique_ptr<LoginResultPacket> pPacket(static_cast<LoginResultPacket*>(_packet.release()));
	if (pPacket->GetLoginResult() == RESULT_TYPE::FAIL)
	{
		pLoginWindow->m_loginResult = "Login Fail!";
		return;
	}

	// 메인 윈도우에 세팅
	MainWindow* pMainWindow = static_cast<MainWindow*>(ImguiWindowManager::GetInst()->GetImguiWindow(WINDOW_UI::MAIN));
	User* pLoginUser = new User(pPacket->GetUserInfo().ID, const_cast<std::vector<std::string>&>(pPacket->GetUserInfo().FriendList));
	pMainWindow->SetLoginUser(pLoginUser);

	// 유저가 접속하지 않은동안 들어온 메세지 처리
	const std::vector<ChatMessage>& unreadMessageList = pPacket->GetUserInfo().UnreadMessage;
	for (const ChatMessage& chatMessage : unreadMessageList)
	{
		ChatWindow::DispatchRecvChatting(chatMessage);
	}

	// 로그인이 성공한 경우
	ImguiWindowManager::GetInst()->ChangeMainWindow(WINDOW_UI::MAIN);
}

void LoginWindow::UpdateWindow()
{
	ImGui::SetNextWindowSize(ImVec2(250, 250), ImGuiCond_FirstUseEver);
	ImGui::Begin("LoginWindow");

	ImGui::Text("ID");
	ImGui::InputText("##ID", m_IDBuffer, BUFFER_SIZE);
	ImGui::Text("Password");
	ImGui::InputText("##Password", m_PWBuffer, BUFFER_SIZE);

	if (ImGui::Button("Login", ImVec2(80, 40)))
	{
		LoginPacket loginPacket(m_IDBuffer, m_PWBuffer);
		if (loginPacket.GetID().empty() == false)
		{
			NetworkManager::GetInst()->Send(&loginPacket);
			m_loginResult.clear();
		}
		else
		{
			m_loginResult = "ID is empty!";
		}
	}

	ImGui::SameLine();
	if (ImGui::Button("Join", ImVec2(80, 40)))
	{
		m_joinModal.Active();
	}

	ImGui::SameLine();
	ImGui::Text(m_loginResult.c_str());

	// JoinModal
	m_joinModal.UpdateWindow();

	ImGui::End();
}
