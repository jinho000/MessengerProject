#include "JoinWindow.h"
#include "imgui.h"
#include <PacketLibrary/JoinPacket.h>
#include <PacketLibrary/JoinResultPacket.h>
#include <PacketLibrary/IDCheckPacket.h>
#include <PacketLibrary/IDCheckResultPacket.h>
#include "ImguiWindowManager.h"
#include "LoginWindow.h"


JoinWindow::JoinWindow()
	: m_IDBuffer()
	, m_PWBuffer()
	, m_active(false)
	, m_bCheckID(false)
	, m_checkResult()
	, m_JoinResult()
{

}

JoinWindow::~JoinWindow()
{
}


void JoinWindow::DispatchJoinResultPacket(std::unique_ptr<PacketBase> _packet)
{
	std::unique_ptr<JoinResultPacket> pPacket(static_cast<JoinResultPacket*>(_packet.release()));

	LoginWindow* pLoginWindow = static_cast<LoginWindow*>(ImguiWindowManager::GetInst()->GetImguiWindow(WINDOW_UI::LOGIN));
	JoinWindow* pJoinWindow = pLoginWindow->GetJoinWindow();

	if (pPacket->GetJoinResult() == RESULT_TYPE::SUCCESS)
	{
		pJoinWindow->m_JoinResult = "Join OK";
	}
	else
	{
		pJoinWindow->m_JoinResult = "Join Fail";
	}
}

void JoinWindow::DispatchIDCheckResultPacket(std::unique_ptr<PacketBase> _packet)
{
	std::unique_ptr<IDCheckResultPacket> pPacket(static_cast<IDCheckResultPacket*>(_packet.release()));

	LoginWindow* pLoginWindow = static_cast<LoginWindow*>(ImguiWindowManager::GetInst()->GetImguiWindow(WINDOW_UI::LOGIN));
	JoinWindow* pJoinWindow = pLoginWindow->GetJoinWindow();

	if (pPacket->GetIDCheckResult() == RESULT_TYPE::SUCCESS)
	{
		pJoinWindow->m_checkResult = "OK";
		pJoinWindow->m_bCheckID = true;
	}
	else
	{
		pJoinWindow->m_checkResult = "Fail";
	}
}


void JoinWindow::UpdateWindow()
{
	if (m_active == false)
		return;

	ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);
	if (ImGui::BeginPopupModal("JoinWindow", &m_active))
	{
		ImGui::Text("ID");
		ImGui::InputText("##ID", m_IDBuffer, BUFFER_SIZE);

		ImGui::SameLine();
		if (ImGui::Button("CheckID"))
		{
			IDCheckPacket packet(m_IDBuffer);
			if (packet.GetID().empty() == true)
			{
				m_checkResult = "ID is Empty!";
			}
			else
			{
				NetworkManager::GetInst()->Send(&packet);
				m_JoinResult = "";
			}
		}

		ImGui::SameLine();
		ImGui::Text(m_checkResult.c_str());

		ImGui::Text("Password");
		ImGui::InputText("##Password", m_PWBuffer, BUFFER_SIZE);

		if (ImGui::Button("Join##2", ImVec2(60, 30)))
		{
			if (m_bCheckID == true)
			{
				JoinPacket packet(m_IDBuffer, m_PWBuffer);
				NetworkManager::GetInst()->Send(&packet);
			}
			else
			{
				m_JoinResult = "check ID!";
			}
		}

		ImGui::SameLine();
		if (ImGui::Button("Close##2", ImVec2(60, 30)))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();
		ImGui::Text(m_JoinResult.c_str());

		ImGui::EndPopup();
	}

}

void JoinWindow::Active()
{
	ImGui::OpenPopup("JoinWindow");
	m_active = true;	
	m_bCheckID = false;

	memset(m_IDBuffer, 0, BUFFER_SIZE);
	memset(m_PWBuffer, 0, BUFFER_SIZE);

	m_checkResult.clear();
	m_JoinResult.clear();
}
