#include "JoinWindow.h"
#include "imgui.h"

JoinWindow::JoinWindow()
	: m_IDBuffer()
	, m_PWBuffer()
	, m_active(false)
	, m_checkResult()
	, m_JoinResult()
{
}

JoinWindow::~JoinWindow()
{
}

void JoinWindow::ShowResultModal()
{
	//bool unused_open = true;
	//if (ImGui::BeginPopupModal("Result", &unused_open))
	//{
	//	ImGui::Text(m_result.c_str());
	//	if (ImGui::Button("OK"))
	//	{
	//		ImGui::CloseCurrentPopup();
	//	}

	//	ImGui::EndPopup();
	//}
}

void JoinWindow::UpdateWindow()
{
	if (m_active == false)
		return;

	if (ImGui::BeginPopupModal("JoinWindow", &m_active))
	{
		ImGui::Text("ID");
		ImGui::InputText("##ID", m_IDBuffer, BUFFER_SIZE);

		ImGui::SameLine();
		if (ImGui::Button("CheckID"))
		{
			// 서버에 ID 전송

			// 결과값 처리
			m_checkResult = "OK"; 
		}

		ImGui::SameLine();
		ImGui::Text(m_checkResult.c_str());

		ImGui::Text("Password");
		ImGui::InputText("##Password", m_PWBuffer, BUFFER_SIZE);

		if (ImGui::Button("Join##2", ImVec2(60, 30)))
		{

			// 서버에 전송하기


			// 서버의 결과를 받아서 처리
			m_JoinResult = "OK";
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

	ShowResultModal();
}

void JoinWindow::Active()
{
	ImGui::OpenPopup("JoinWindow");
	m_active = true;	

	memset(m_IDBuffer, 0, BUFFER_SIZE);
	memset(m_PWBuffer, 0, BUFFER_SIZE);

	m_checkResult.clear();
	m_JoinResult.clear();
}
