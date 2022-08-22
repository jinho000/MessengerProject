#include "pch.h"
#include "DispatchFunction.h"
#include <PacketLibrary/PacketHeader.h>
#include "TCPSession.h"
#include "UserManager.h"

void DispatchLoginPacket(TCPSession* _TCPSession, std::unique_ptr<PacketBase> _loginPacket)
{
	std::unique_ptr<LoginPacket> pLoginPacket(static_cast<LoginPacket*>(_loginPacket.release()));

	// 서버에 접속처리
	UserManager::GetInst()->AddUser(pLoginPacket->GetID(), _TCPSession);


	// DB처리
	UserInfo userInfo;
	userInfo.ID = pLoginPacket->GetID();
	userInfo.PW = pLoginPacket->GetPW();

	userInfo.FriendList.push_back("ID1");
	userInfo.FriendList.push_back("ID2");
	userInfo.FriendList.push_back("ID3");
	userInfo.FriendList.push_back("ID4");


	// DB결과를 받아서 처리하기
	LoginResultPacket resultPacket(RESULT_TYPE::SUCCESS, userInfo);
	_TCPSession->Send(&resultPacket);
}

void DispatchJoinPacket(TCPSession* _TCPSession, std::unique_ptr<PacketBase> _joinPacket)
{
	std::unique_ptr<JoinPacket> pJoinPacket(static_cast<JoinPacket*>(_joinPacket.release()));

	// DB 처리


	// DB결과를 받아서 처리하기
	JoinResultPacket resultPacket(RESULT_TYPE::SUCCESS);
	_TCPSession->Send(&resultPacket);
}

void DispatchIDCheckPacket(TCPSession* _TCPSession, std::unique_ptr<PacketBase> _IDCheckPacket)
{

	// DB에서 확인하기


	// DB결과를 받아서 처리하기
	IDCheckResultPacket resultPacket(RESULT_TYPE::SUCCESS);
	_TCPSession->Send(&resultPacket);
}

void DispatchAddFriendPacket(TCPSession* _TCPSession, std::unique_ptr<PacketBase> _AddFriendPacket)
{
	std::unique_ptr<AddFriendResultPacket> pAddFriendResultPacket(static_cast<AddFriendResultPacket*>(_AddFriendPacket.release()));

	// DB처리


	// DB결과를 받아서 처리하기
	AddFriendResultPacket resultPacket(pAddFriendResultPacket->GetFriendID(), RESULT_TYPE::SUCCESS);
	_TCPSession->Send(&resultPacket);
}

void DispatchSendChattingPacket(TCPSession* _TCPSession, std::unique_ptr<PacketBase> _sendChattingPacket)
{
	std::unique_ptr<SendChattingPacket> pSendChattingPacket(static_cast<SendChattingPacket*>(_sendChattingPacket.release()));

	TCPSession* pRecvUserSession = UserManager::GetInst()->FindUser(pSendChattingPacket->GetRecvUserID());
	
	if (nullptr == pRecvUserSession)
	{
		return;
	}

	RecvChattingPacket recvPacket(pSendChattingPacket->GetSendUserID(), pSendChattingPacket->GetRecvUserID(), pSendChattingPacket->GetChatMessage());
	pRecvUserSession->Send(&recvPacket);
}

