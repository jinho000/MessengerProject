#pragma once
#include <PacketLibrary/LoginPacket.h>
#include <PacketLibrary/LoginResultPacket.h>
#include "TCPSession.h"


class TCPSession;
void LoginPacketHandler(TCPSession* _TCPSession, std::unique_ptr<PacketBase> _loginPacket)
{
	std::unique_ptr<LoginPacket> pLoginPacket(static_cast<LoginPacket*>(_loginPacket.release()));


	// DBÃ³¸®
	UserInfo userInfo;
	userInfo.ID = pLoginPacket->GetID();
	userInfo.PW = pLoginPacket->GetPW();

	userInfo.FriendList.push_back("ID1");
	userInfo.FriendList.push_back("ID2");
	userInfo.FriendList.push_back("ID3");
	userInfo.FriendList.push_back("ID4");

	LoginResultPacket resultPacket(RESULT_TYPE::SUCCESS, userInfo);
	_TCPSession->RequestSend(&resultPacket);
}

