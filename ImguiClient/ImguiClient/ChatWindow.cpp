#include "ChatWindow.h"
#include "imgui.h"


ChatWindow::ChatWindow()
	: m_friendID()
	, m_bActive(false)
{
}

void ChatWindow::UpdateWindow()
{
	ImGui::Begin(m_friendID.c_str(), &m_bActive);



	ImGui::Separator();

	ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue;
	char buff[255] = {};
	if (ImGui::InputText(" ##ss", buff, 255, input_text_flags))
	{
		int a = 0;
	}

	ImGui::End();
}

