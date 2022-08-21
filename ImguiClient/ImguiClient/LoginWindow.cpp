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
	std::unique_ptr<LoginResultPacket> pPacket(static_cast<LoginResultPacket*>(_packet.release()));

	User* pLoginUser = new User(pPacket->GetUserInfo().ID, const_cast<std::vector<std::string>&>(pPacket->GetUserInfo().FriendList));

	// 메인 윈도우에 세팅
	MainWindow* pMainWindow = static_cast<MainWindow*>(ImguiWindowManager::GetInst()->GetImguiWindow(WINDOW_UI::MAIN));
	pMainWindow->SetLoginUser(pLoginUser);

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
		NetworkManager::GetInst()->Send(&loginPacket);
	}

	ImGui::SameLine();
	if (ImGui::Button("Join", ImVec2(80, 40)))
	{
		m_joinModal.Active();
	}


	// JoinModal
	m_joinModal.UpdateWindow();

	ImGui::End();
}
