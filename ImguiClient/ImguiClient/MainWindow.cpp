#include "MainWindow.h"
#include "imgui.h"
#include "User.h"
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

	// 채팅창 출력
	for (size_t i = 0; i < m_chatWindowList.size(); i++)
	{
		m_chatWindowList[i]->UpdateWindow();
	}


	// 유저 정보 출력
	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("TabBar", tab_bar_flags))
	{
		const std::vector<std::string>& chatFriendList = m_pLoginUser->GetChatFriendList();

		if (ImGui::BeginTabItem("Friend List"))
		{
			int selected = -2;
			
			// 유저			
			std::string print(m_pLoginUser->GetUserNickname());
			print += '\n';

			if (ImGui::Selectable(print.c_str(), selected == -1))
				selected = -1;

			ImGui::Separator();

			// 친구목록
			for (size_t i = 0; i < chatFriendList.size(); i++)
			{
				std::string friendName(chatFriendList[i]);
				friendName += '\n';

				if (ImGui::Selectable(friendName.c_str(), selected == i))
					selected = (int)i;

				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
				{
					

				}

				ImGui::Separator();
			}

			

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Chatting List"))
		{
			for (size_t i = 0; i < chatFriendList.size(); i++)
			{
				//if (chatFriendList[i]->GetMessageList().empty() == true)
				//	continue;

				//int selected = -1;
				//std::string name = chatFriendList[i]->GetChatFriendName();
				//const ChatMessage& chatMessage = chatFriendList[i]->GetMessageList().front();

				//std::string print = name + "\n" + chatMessage.message;
				//if (ImGui::Selectable(print.c_str(), selected == i))
				//	selected = (int)i;

				//if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
				//{
				//	ChatFriend* pChatFriend = chatFriendList[i];
				//	m_chatWindowList[i]->SetChatFriend(pChatFriend);
				//}

				ImGui::Separator();
			}
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	// 로그아웃 버튼
	if (ImGui::Button("logout"))
	{
		ImguiWindowManager::GetInst()->ChangeMainWindow(WINDOW_UI::LOGIN);
	}

	ImGui::End();
}


void MainWindow::CreateChatWindow(const std::string& _friend)
{
	if (m_charWindowMap.end() != m_charWindowMap.find(_friend))
	{
		return;
	}

	ChatWindow* pChatWindow = new ChatWindow();
	m_chatWindowList.push_back(pChatWindow);
	m_charWindowMap.insert(make_pair(_friend, pChatWindow));
}

void MainWindow::SetLoginUser(User* _pLoginUser)
{
	m_pLoginUser = _pLoginUser;
}
