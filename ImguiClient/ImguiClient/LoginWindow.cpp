#include "LoginWindow.h"
#include "imgui.h"
#include "ImguiWindowManager.h"
#include "User.h"
#include "MainWindow.h"
#include "NetworkManager.h"

#include <PacketLibrary/LoginPacket.h>

LoginWindow::LoginWindow()
	: m_IDBuffer()
	, m_PWBuffer()
{
}

void LoginWindow::UpdateWindow()
{
	ImGui::Begin("LoginWindow");

	ImGui::Text("ID");
	ImGui::InputText("##ID", m_IDBuffer, BUFFER_SIZE);
	ImGui::Text("Password");
	ImGui::InputText("##Password", m_PWBuffer, BUFFER_SIZE);

	if (ImGui::Button("Login", ImVec2(80, 40)))
	{
		LoginPacket loginPacket(m_IDBuffer, m_PWBuffer);

		std::vector<std::string> friendList;
		friendList.push_back("ID1");
		friendList.push_back("ID2");
		friendList.push_back("ID3");
		friendList.push_back("ID4");

		User* pLoginUser = new User(m_IDBuffer, "nickname", friendList);

		// 메인 윈도우에 세팅
		MainWindow* pMainWindow = static_cast<MainWindow*>(ImguiWindowManager::GetInst()->GetImguiWindow(WINDOW_UI::MAIN));
		pMainWindow->SetLoginUser(pLoginUser);

		// 로그인이 성공한 경우
		ImguiWindowManager::GetInst()->ChangeMainWindow(WINDOW_UI::MAIN);

		// 로그인이 성공한 경우
		//ImguiWindowManager::GetInst()->ChangeMainWindow(WINDOW_UI::MAIN);

		//if (NetworkManager::GetInst()->ConnectServer())
		//{
		//	NetworkManager::GetInst()->Send(&loginPacket);
		//}
		//else
		//{
		//	ImGui::OpenPopup("Error! ##ConnectServerFail");

		//	// Always center this window when appearing
		//	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		//	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		//}

	}

	// popup
	if (ImGui::BeginPopupModal("Error! ##ConnectServerFail", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Connect Server Fail\n\n");
		ImGui::Separator();

		if (ImGui::Button("OK", ImVec2(120, 0))) 
		{ 
			ImGui::CloseCurrentPopup(); 
		}

		ImGui::EndPopup();
	}

	ImGui::End();
}
