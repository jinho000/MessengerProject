#include "MainWindow.h"
#include "imgui.h"
#include "User.h"
#include "ImguiWindowManager.h"

MainWindow::MainWindow()
	: m_pLoginUser(nullptr)
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

		// 친구목록
		if (ImGui::BeginTabItem("Friend List"))
		{
			int selected = -1;
			for (size_t i = 0; i < chatFriendList.size(); i++)
			{
				std::string friendName(chatFriendList[i]);
				friendName += '\n';

				if (ImGui::Selectable(friendName.c_str(), selected == i))
					selected = (int)i;

				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
				{
					CreateChatWindow(friendName);
				}

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
	auto iter = m_charWindowMap.find(_friend);
	if (iter != m_charWindowMap.end())
	{
		iter->second->Active();
		return;
	}
	
	ChatWindow* pChatWindow = new ChatWindow(_friend);
	m_chatWindowList.push_back(pChatWindow);
	m_charWindowMap.insert(make_pair(_friend, pChatWindow));
}

void MainWindow::SetLoginUser(User* _pLoginUser)
{
	m_pLoginUser = _pLoginUser;
}
