#include "MainWindow.h"
#include "imgui.h"
#include "User.h"
#include "ImguiWindowManager.h"
#include "NetworkManager.h"

#include <PacketLibrary/AddFriendPacket.h>
#include <PacketLibrary/AddFriendResultPacket.h>

MainWindow::MainWindow()
	: m_pLoginUser(nullptr)
	, m_bAddFriendPopup(false)
	, m_addFriendID()
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


void MainWindow::DispatchAddFriendResultPacket(std::unique_ptr<PacketBase> _packet)
{
	std::unique_ptr<AddFriendResultPacket> pPacket(static_cast<AddFriendResultPacket*>(_packet.release()));

	// 메인 윈도우에 세팅
	MainWindow* pMainWindow = static_cast<MainWindow*>(ImguiWindowManager::GetInst()->GetImguiWindow(WINDOW_UI::MAIN));

	if (pPacket->GetAddFriendResult() == RESULT_TYPE::SUCCESS)
	{
		pMainWindow->m_addFriendResult = "Add Success";
		const std::string& friendID = pPacket->GetFriendID();
		pMainWindow->m_pLoginUser->AddFriend(friendID);
	}
	else
	{
		pMainWindow->m_addFriendResult = "Add Fail";
	}
}



void MainWindow::UpdateWindow()
{
	ImGui::SetNextWindowSize(ImVec2(300, 450), ImGuiCond_FirstUseEver);
	ImGui::Begin("MainWindow");

	// 채팅창 업데이트
	for (size_t i = 0; i < m_chatWindowList.size(); i++)
	{
		m_chatWindowList[i]->UpdateWindow();
	}

	// 친구 추가 버튼
	if (ImGui::Button("Add Friend##Button", ImVec2(80, 40)))
	{
		ShowAddFriendPopup();
	}
	ImGui::SameLine();

	// 로그아웃버튼
	if (ImGui::Button("Logout", ImVec2(80, 40)))
	{
		m_bLogout = true;
	}

	ImGui::Separator();

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

		// 채팅목록
		if (ImGui::BeginTabItem("Chatting List"))
		{
			int selected = -1;
			for (size_t i = 0; i < m_chatWindowList.size(); i++)
			{
				std::string print(m_chatWindowList[i]->GetFriendID());
				print += '\n';
				print += m_chatWindowList[i]->GetFirstMessage();

				if (ImGui::Selectable(print.c_str(), selected == i))
					selected = (int)i;

				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
				{
					m_chatWindowList[i]->Active();
				}

				ImGui::Separator();
			}

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}


	// 친구추가 팝업 업데이트
	UpdateAddFriendPopup();

	// 로그아웃 함수 실행
	if (m_bLogout == true)
	{
		LogoutUser();
		m_bLogout = false;
	}

	ImGui::End();
}


void MainWindow::CreateChatWindow(const std::string& _friend)
{
	for (const auto& chatWindow : m_chatWindowList)
	{
		if (chatWindow->GetFriendID() == _friend)
		{
			chatWindow->Active();
			return;
		}
	}

	ChatWindow* pChatWindow = new ChatWindow(_friend);
	m_chatWindowList.push_back(pChatWindow);
}

void MainWindow::ShowAddFriendPopup()
{
	ImGui::OpenPopup("Add Friend");
	m_bAddFriendPopup = true;
	m_bPopupInputFocus = true;
	m_addFriendResult.clear();
	memset(m_addFriendID, 0, 255);
}

void MainWindow::UpdateAddFriendPopup()
{
	if (m_bAddFriendPopup == false)
		return;

	ImGui::SetNextWindowSize(ImVec2(200, 140), ImGuiCond_FirstUseEver);
	if (ImGui::BeginPopupModal("Add Friend", &m_bAddFriendPopup))
	{
		ImGui::Text("Friend ID");

		if (ImGui::InputText("##Friend ID", m_addFriendID, 255, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			// 서버로 아이디 전송
			AddFriendPacket packet(m_addFriendID);
			NetworkManager::GetInst()->Send(&packet);

			m_bPopupInputFocus = true;
		}

		// 입력창 포커싱
		ImGui::SetItemDefaultFocus();
		if (m_bPopupInputFocus == true)
		{
			ImGui::SetKeyboardFocusHere(-1);
			m_bPopupInputFocus = false;
		}

		ImGui::SameLine();
		if (ImGui::Button("Add", ImVec2(40, 20)))
		{
			// 서버에 아이디 전송
			AddFriendPacket packet(m_addFriendID);
			NetworkManager::GetInst()->Send(&packet);
			
			m_bPopupInputFocus = true;
		}

		ImGui::Separator();

		ImGui::Text(m_addFriendResult.c_str());

		ImGui::EndPopup();
	}
}

void MainWindow::LogoutUser()
{
	// 현재 유저 제거
	delete m_pLoginUser;
	m_pLoginUser = nullptr;

	// 현재 채팅창 제거
	DestroyChatWindow();

	// 로그인 UI로 이동
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
}

void MainWindow::SetLoginUser(User* _pLoginUser)
{
	m_pLoginUser = _pLoginUser;
}
