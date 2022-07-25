#include "MainWindow.h"
#include "imgui.h"

MainWindow::MainWindow()
	: m_mainUIType(MAIN_UI_TYPE::CHATWINDOW_LIST)
	, m_loginUser(nullptr)
{
}

MainWindow::~MainWindow()
{
	delete m_loginUser;
	m_loginUser = nullptr;
}

void MainWindow::UpdateWindow()
{
	ImGui::Begin("MainWindow");

	if (m_mainUIType == MAIN_UI_TYPE::CHATWINDOW_LIST)
	{


	}
	else
	{

	}

	ImGui::End();
}
