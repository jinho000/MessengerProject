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
		// ���� ������ ������ ��Ŷ ����
		// send m_IDBuffer

		LoginPacket loginPacket(m_IDBuffer, "pw");
		NetworkManager::GetInst()->Send(&loginPacket);

		// recv
		// 
		// ���� ������ �޴´�
		// ä�� ������ �޴´�
		std::string loginID = "userTest";
		std::string name = "name";


		std::vector<ChatMessage> messageList;
		messageList.push_back({ "userTest", "message1" });
		messageList.push_back({ "ID1", "message2" });

		std::vector<ChatMessage> messageList2 = (messageList);


		std::vector<ChatFriend*> chatFriendList;
		chatFriendList.push_back(new ChatFriend("ID1", messageList));
		chatFriendList.push_back(new ChatFriend("ID2", messageList2));
		chatFriendList.push_back(new ChatFriend("ID3"));
		chatFriendList.push_back(new ChatFriend("ID4"));

		User* pLoginUser = new User(loginID, name, chatFriendList);
		

		// ���� �����쿡 ����
		MainWindow* pMainWindow = static_cast<MainWindow*>(ImguiWindowManager::GetInst()->GetImguiWindow(WINDOW_UI::MAIN));
		pMainWindow->SetLoginUser(pLoginUser);


		// �α����� ������ ���
		ImguiWindowManager::GetInst()->ChangeMainWindow(WINDOW_UI::MAIN);
	}

	ImGui::End();
}
