#include "LoginWindow.h"
#include "imgui.h"
#include "ImguiWindowManager.h"
#include "User.h"
#include "MainWindow.h"
#include "ChatFriend.h"
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
		NetworkManager::GetInst()->Send(&loginPacket);
	}

	ImGui::End();
}
