#include "LoginWindow.h"
#include "imgui.h"
#include "ImguiWindowManager.h"
#include "User.h"
#include "MainWindow.h"

LoginWindow::LoginWindow()
	: m_IDBuffer()
{
}

void LoginWindow::UpdateWindow()
{
	ImGui::Begin("LoginWindow");

	ImGui::Text("ID");
	ImGui::InputText("##ID", m_IDBuffer, 64);
	//ImGui::Text("Password");
	//ImGui::InputText("##Password", buf2, 64);

	if (ImGui::Button("Login", ImVec2(80, 40)))
	{
		// 서버 연동시 서버에 패킷 전송
		// send m_IDBuffer


		// 메인 윈도우에 세팅
		MainWindow* pMainWindow = static_cast<MainWindow*>(ImguiWindowManager::GetInst()->GetImguiWindow(WINDOW_UI::MAIN));


		// recv
		// 유저 정보를 받는다
		User* pLoginUser = new User;
		

		// 채팅 정보를 받는다


		// 로그인이 성공한 경우
		ImguiWindowManager::GetInst()->ChangeMainWindow(WINDOW_UI::MAIN);
	}

	ImGui::End();
}
