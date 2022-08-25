#include "pch.h"
#include "DispatchFunction.h"
#include <PacketLibrary/PacketHeader.h>
#include "TCPSession.h"
#include "UserManager.h"

void DispatchLoginPacket(TCPSession* _TCPSession, std::unique_ptr<PacketBase> _loginPacket)
{
	std::unique_ptr<LoginPacket> pLoginPacket(static_cast<LoginPacket*>(_loginPacket.release()));

	// ������ ����ó��
	UserManager::GetInst()->AddUser(pLoginPacket->GetID(), _TCPSession);


	// DBó��
	UserInfo userInfo;
	userInfo.ID = pLoginPacket->GetID();
	userInfo.PW = pLoginPacket->GetPW();

	userInfo.FriendList.push_back("ID1");
	userInfo.FriendList.push_back("ID2");
	userInfo.FriendList.push_back("ID3");
	userInfo.FriendList.push_back("ID4");

	// ������ �������� ��ϵ� �޼����� ������� ä����Ŷ ������
	std::vector<ChatMessage>* unreadChattingList = UserManager::GetInst()->GetUnreadMessageList(pLoginPacket->GetID());
	if (nullptr != unreadChattingList)
	{
		userInfo.UnreadMessage = std::move(*unreadChattingList);
	}

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
	std::unique_ptr<ChattingPacket> pSendChattingPacket(static_cast<ChattingPacket*>(_sendChattingPacket.release()));

	TCPSession* pRecvUserSession = UserManager::GetInst()->FindUser(pSendChattingPacket->GetChattingMessage().recvUserID);
	
	// ������ ���°��, ������ �������� �������
	// ��ϵ��� ���� �������Դ� ä���� ���� �� ����
	// ������ �޼��� ����
	if (nullptr == pRecvUserSession)
	{
		std::string recvUserID = pSendChattingPacket->GetChattingMessage().recvUserID;
		UserManager::GetInst()->AddUnreadChatting(recvUserID, std::move(pSendChattingPacket));
		return;
	}

	ChattingPacket recvPacket(pSendChattingPacket->GetChattingMessage());
	pRecvUserSession->Send(&recvPacket);
}

void DispatchLogoutPacket(TCPSession* _TCPSession, std::unique_ptr<PacketBase> _logoutPacket)
{
	std::unique_ptr<LogoutPacket> pLogoutPacket(static_cast<LogoutPacket*>(_logoutPacket.release()));
	UserManager::GetInst()->DeleteUser(pLogoutPacket->GetLogoutID());
}

void DispatchReadChattingPacket(TCPSession* _TCPSession, std::unique_ptr<PacketBase> _readChattingPacket)
{
	std::unique_ptr<ReadChattingPacket> pReadChattingPacket(static_cast<ReadChattingPacket*>(_readChattingPacket.release()));

	TCPSession* pRecvUserSession = UserManager::GetInst()->FindUser(pReadChattingPacket->GetSendUserID());

	// ������ �������� ������� ä���� �������� �����Ƿ� �׳� ����
	// ä�ó����� ����� ������ �־����
	if (nullptr == pRecvUserSession)
	{
		return;
	}

	pRecvUserSession->Send(pReadChattingPacket.get());
}

