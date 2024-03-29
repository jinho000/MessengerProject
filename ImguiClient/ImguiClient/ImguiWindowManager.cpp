#include "ImguiWindowManager.h"
#include "imgui.h"

#include "LoginWindow.h"
#include "MainWindow.h"
#include "ServerConnectModal.h"

ImguiWindowManager* ImguiWindowManager::pInst = nullptr;

ImguiWindowManager::ImguiWindowManager()
	: m_imguiWindow()
	, m_currentUI(WINDOW_UI::LOGIN)
{
	// 로그인창, 메인창 추가
	m_imguiWindow[static_cast<int>(WINDOW_UI::LOGIN)] = new LoginWindow;
	m_imguiWindow[static_cast<int>(WINDOW_UI::MAIN)] = new MainWindow;

	m_serverConnectModal = new ServerConnectModal;
}

ImguiWindowManager::~ImguiWindowManager()
{
	for (size_t i = 0; i < static_cast<int>(WINDOW_UI::END); i++)
	{
		delete m_imguiWindow[i];
		m_imguiWindow[i] = nullptr;
	}

	delete m_serverConnectModal;
	m_serverConnectModal = nullptr;
}

void ImguiWindowManager::CreateIntance()
{
	if (pInst != nullptr)
		return;

	pInst = new ImguiWindowManager;
}

void ImguiWindowManager::Destroy()
{
	if (pInst != nullptr)
	{
		delete pInst;
		pInst = nullptr;
	}
}

void ImguiWindowManager::UpdateWindow()
{
	m_imguiWindow[static_cast<int>(m_currentUI)]->UpdateWindow();

	m_serverConnectModal->UpdateWindow();
}

void ImguiWindowManager::ChangeMainWindow(WINDOW_UI _window)
{
	m_currentUI = _window;
}

