#include "pch.h"
#include "DispatchFunction.h"
#include <PacketLibrary/PacketHeader.h>
#include "TCPSession.h"

void DispatchLoginPacket(TCPSession* _TCPSession, std::unique_ptr<PacketBase> _loginPacket)
{
	std::unique_ptr<LoginPacket> pLoginPacket(static_cast<LoginPacket*>(_loginPacket.release()));


	// DBó��
	UserInfo userInfo;
	userInfo.ID = pLoginPacket->GetID();
	userInfo.PW = pLoginPacket->GetPW();

	userInfo.FriendList.push_back("ID1");
	userInfo.FriendList.push_back("ID2");
	userInfo.FriendList.push_back("ID3");
	userInfo.FriendList.push_back("ID4");


	// DB����� �޾Ƽ� ó���ϱ�
	LoginResultPacket resultPacket(RESULT_TYPE::SUCCESS, userInfo);
	_TCPSession->Send(&resultPacket);
}

void DispatchJoinPacket(TCPSession* _TCPSession, std::unique_ptr<PacketBase> _joinPacket)
{
	std::unique_ptr<JoinPacket> pJoinPacket(static_cast<JoinPacket*>(_joinPacket.release()));

	// DB ó��


	// DB����� �޾Ƽ� ó���ϱ�
	JoinResultPacket resultPacket(RESULT_TYPE::SUCCESS);
	_TCPSession->Send(&resultPacket);
}

void DispatchIDCheckPacket(TCPSession* _TCPSession, std::unique_ptr<PacketBase> _IDCheckPacket)
{

	// DB���� Ȯ���ϱ�


	// DB����� �޾Ƽ� ó���ϱ�
	IDCheckResultPacket resultPacket(RESULT_TYPE::SUCCESS);
	_TCPSession->Send(&resultPacket);
}

void DispatchAddFriendPacket(TCPSession* _TCPSession, std::unique_ptr<PacketBase> _AddFriendPacket)
{
	std::unique_ptr<AddFriendResultPacket> pAddFriendResultPacket(static_cast<AddFriendResultPacket*>(_AddFriendPacket.release()));

	// DBó��


	// DB����� �޾Ƽ� ó���ϱ�
	AddFriendResultPacket resultPacket(pAddFriendResultPacket->GetFriendID(), RESULT_TYPE::SUCCESS);
	_TCPSession->Send(&resultPacket);
}

void DispatchSendChattingPacket(TCPSession* _TCPSession, std::unique_ptr<PacketBase> _sendChattingPacket)
{

}

