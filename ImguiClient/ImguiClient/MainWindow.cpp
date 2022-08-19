#include "MainWindow.h"
#include "imgui.h"
#include "User.h"
#include "ChatFriend.h"
#include "ImguiWindowManager.h"

MainWindow::MainWindow()
	: m_mainUIType(MAIN_UI_TYPE::CHATWINDOW_LIST)
	, m_pLoginUser(nullptr)
{
}

MainWindow::~MainWindow()
{
	if (m_pLoginUser != nullptr)
	{
		delete m_pLoginUser;
		m_pLoginUser = nullptr;
	}

	for (size_t i = 0; i < m_chatWindowList.size(); i++)
	{
		delete m_chatWindowList[i];
		m_chatWindowList[i] = nullptr;
	}
}


void MainWindow::UpdateWindow()
{
	ImGui::Begin("MainWindow");

	// ä��â ���
	for (size_t i = 0; i < m_chatWindowList.size(); i++)
	{
		m_chatWindowList[i]->UpdateWindow();
	}


	// ���� ���� ���
	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("TabBar", tab_bar_flags))
	{
		const std::vector<std::string>& chatFriendList = m_pLoginUser->GetChatFriendList();

		if (ImGui::BeginTabItem("Friend List"))
		{
			int selected = -2;
			
			// ����			
			std::string print(m_pLoginUser->GetUserName());
			print += '\n';

			if (ImGui::Selectable(print.c_str(), selected == -1))
				selected = -1;

			ImGui::Separator();

			// ģ�����
			for (size_t i = 0; i < chatFriendList.size(); i++)
			{
				std::string friendName(chatFriendList[i]);
				friendName += '\n';

				if (ImGui::Selectable(friendName.c_str(), selected == i))
					selected = (int)i;

				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
				{
					ChatFriend* pChatFriend = chatFriendList[i];
					m_chatWindowList[i]->SetChatFriend(pChatFriend);
				}

				ImGui::Separator();
			}

			

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Chatting List"))
		{
			for (size_t i = 0; i < chatFriendList.size(); i++)
			{
				if (chatFriendList[i]->GetMessageList().empty() == true)
					continue;

				int selected = -1;
				std::string name = chatFriendList[i]->GetChatFriendName();
				const ChatMessage& chatMessage = chatFriendList[i]->GetMessageList().front();

				std::string print = name + "\n" + chatMessage.message;
				if (ImGui::Selectable(print.c_str(), selected == i))
					selected = (int)i;

				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
				{
					ChatFriend* pChatFriend = chatFriendList[i];
					m_chatWindowList[i]->SetChatFriend(pChatFriend);
				}

				ImGui::Separator();
			}
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	// �α׾ƿ� ��ư
	if (ImGui::Button("logout"))
	{
		ImguiWindowManager::GetInst()->ChangeMainWindow(WINDOW_UI::LOGIN);
	}

	ImGui::End();
}

void MainWindow::CreateChatWindow(int _count)
{
	for (size_t i = 0; i < _count; i++)
	{
		m_chatWindowList.push_back(new ChatWindow());
	}
}

void MainWindow::SetLoginUser(User* _pLoginUser)
{
	m_pLoginUser = _pLoginUser;

	CreateChatWindow((int)m_pLoginUser->GetChatFriendList().size());
}
