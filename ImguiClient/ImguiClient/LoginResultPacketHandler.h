#pragma once
#include <PacketLibrary/LoginResultPacket.h>
#include <PacketLibrary/struct.h>

#include "MainWindow.h"
#include "ImguiWindowManager.h"


void LoginResultPacketHandler(std::unique_ptr<PacketBase> _packetBase)
{
	std::unique_ptr<LoginResultPacket> pPacket(static_cast<LoginResultPacket*>(_packetBase.release()));

	//std::vector<std::string> friendList = pPacket->GetUserInfo().FriendList;
	//User* pLoginUser = new User(pPacket->GetUserInfo().ID, pPacket->GetUserInfo().NickName, friendList);


	//// ���� �����쿡 ����
	//MainWindow* pMainWindow = static_cast<MainWindow*>(ImguiWindowManager::GetInst()->GetImguiWindow(WINDOW_UI::MAIN));
	//pMainWindow->SetLoginUser(pLoginUser);


	// �α����� ������ ���
	ImguiWindowManager::GetInst()->ChangeMainWindow(WINDOW_UI::MAIN);
}
