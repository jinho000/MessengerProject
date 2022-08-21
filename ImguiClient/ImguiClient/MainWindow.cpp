#include "MainWindow.h"
#include "imgui.h"
#include "User.h"
#include "ImguiWindowManager.h"

MainWindow::MainWindow()
	: m_pLoginUser(nullptr)
	, m_bAddFriendPopup(false)
	, m_searchFriendID()
	, m_bPopupInputFocus(false)
	, m_bLogout(false)
{
}

MainWindow::~MainWindow()
{
	if (m_pLoginUser != nullptr)
	{
		delete m_pLoginUser;
		m_pLoginUser = nullptr;
	}

	DestroyChatWindow();
}

void MainWindow::UpdateWindow()
{
	ImGui::SetNextWindowSize(ImVec2(300, 450), ImGuiCond_FirstUseEver);
	ImGui::Begin("MainWindow");

	// ä��â ������Ʈ
	for (size_t i = 0; i < m_chatWindowList.size(); i++)
	{
		m_chatWindowList[i]->UpdateWindow();
	}

	// ģ�� �߰� ��ư
	if (ImGui::Button("Add Friend", ImVec2(80, 40)))
	{
		ShowAddFriendPopup();
	}
	ImGui::SameLine();

	// �α׾ƿ���ư
	if (ImGui::Button("Logout", ImVec2(80, 40)))
	{
		m_bLogout = true;
	}
	ImGui::Separator();

	// ���� ���� ���
	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("TabBar", tab_bar_flags))
	{
		const std::vector<std::string>& chatFriendList = m_pLoginUser->GetChatFriendList();

		// ģ�����
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


	// ģ���߰� �˾� ������Ʈ
	UpdateAddFriendPopup();

	// �α׾ƿ� �Լ� ����
	if (m_bLogout == true)
	{
		LogoutUser();
		m_bLogout = false;
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

void MainWindow::ShowAddFriendPopup()
{
	ImGui::OpenPopup("Add Friend");
	m_bAddFriendPopup = true;
	m_bPopupInputFocus = true;
	memset(m_searchFriendID, 0, 255);
}

void MainWindow::UpdateAddFriendPopup()
{
	if (m_bAddFriendPopup == false)
		return;

	ImGui::SetNextWindowSize(ImVec2(200, 140), ImGuiCond_FirstUseEver);
	if (ImGui::BeginPopupModal("Add Friend", &m_bAddFriendPopup))
	{
		ImGui::Text("Friend ID");

		if (ImGui::InputText("##Friend ID", m_searchFriendID, 255, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			memset(m_searchFriendID, 0, 255);
			m_bPopupInputFocus = true;

			// ������ ���̵� ����

		}

		ImGui::SetItemDefaultFocus();
		if (m_bPopupInputFocus == true)
		{
			ImGui::SetKeyboardFocusHere(-1);
			m_bPopupInputFocus = false;
		}

		ImGui::SameLine();
		if (ImGui::Button("Add", ImVec2(40, 20)))
		{
			memset(m_searchFriendID, 0, 255);
			m_bPopupInputFocus = true;

			// ������ ���̵� ����
			int a = 0;


		}

		ImGui::Separator();

		ImGui::Text("Add Result ");

		ImGui::EndPopup();
	}
}

void MainWindow::LogoutUser()
{
	// ���� ���� ����
	delete m_pLoginUser;
	m_pLoginUser = nullptr;

	// ���� ä��â ����
	DestroyChatWindow();

	// �α��� UI�� �̵�
	ImguiWindowManager::GetInst()->ChangeMainWindow(WINDOW_UI::LOGIN);
}

void MainWindow::DestroyChatWindow()
{
	for (size_t i = 0; i < m_chatWindowList.size(); i++)
	{
		delete m_chatWindowList[i];
		m_chatWindowList[i] = nullptr;
	}

	m_chatWindowList.clear();
	m_charWindowMap.clear();
}

void MainWindow::SetLoginUser(User* _pLoginUser)
{
	m_pLoginUser = _pLoginUser;
}
