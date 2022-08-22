#include "ServerConnectModal.h"
#include "imgui.h"
#include "NetworkManager.h"

ServerConnectModal::ServerConnectModal()
	: m_bActive(false)
	, m_bConncetServer(false)
{
}

ServerConnectModal::~ServerConnectModal()
{
}

void ServerConnectModal::UpdateWindow()
{
	if (m_bConncetServer == true)
		return;

	if (m_bActive == false && m_bConncetServer == false)
	{
		if (NetworkManager::GetInst()->ConnectServer() == true)
		{
			m_bConncetServer = true;
		}
		else
		{
			m_bActive = true;
			ImGui::OpenPopup("Error##ServerConnectModal");
		}
	}

	ImGui::SetNextWindowSize(ImVec2(180, 100), ImGuiCond_FirstUseEver);
	if (ImGui::BeginPopupModal("Error##ServerConnectModal"))
	{
		ImGui::Text("Connect Server Fail");

		if (ImGui::Button("Connect", ImVec2({ 70, 20 })))
		{
			if (NetworkManager::GetInst()->ReConnectServer() == true)
			{
				m_bConncetServer = true;
				m_bActive = false;
				m_connectResult.clear();
				ImGui::CloseCurrentPopup();
			}
			else
			{
				m_connectResult = "Reconnect Fail";
			}
		}
		ImGui::SameLine();
		ImGui::Text(m_connectResult.c_str());

		ImGui::EndPopup();
	}
}

void ServerConnectModal::Active()
{
	m_bConncetServer = false;
}
