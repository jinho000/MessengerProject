#include "pch.h"
#include "DispatchFunction.h"
#include <PacketLibrary/PacketHeader.h>
#include "TCPSession.h"
#include "UserManager.h"
#include "DBManager.h"

#include "FindIDQuery.h"
#include "JoinQuery.h"
#include "LoginQuery.h"
#include "FriendListQuery.h"

void DispatchLoginPacket(TCPSession* _TCPSession, std::unique_ptr<PacketBase> _loginPacket)
{
	std::unique_ptr<LoginPacket> pLoginPacket(static_cast<LoginPacket*>(_loginPacket.release()));

	// �α��� ó��
	LoginQuery loginQuery(pLoginPacket->GetID(), pLoginPacket->GetPW());
	if (DBManager::GetInst()->DoQueryAndSetResult(loginQuery) == true 
		&& loginQuery.IsLoginSuccess() == true)
	{
		// �α��� ������ �����ϰ� �α��ο� ������ ���, ������ ����ó��
		UserManager::GetInst()->AddUser(pLoginPacket->GetID(), _TCPSession);
		
		// �α��ο� �����ϰ� ������ ID index�� �����°�� ��������Ʈ������ ������ �� ����
		int userIDIndex = loginQuery.GetUserIDIndex();
		FriendListQuery friendListQuery(userIDIndex);
		DBManager::GetInst()->DoQueryAndSetResult(friendListQuery);


		// ���� ���� ��������
		UserInfo userInfo;
		userInfo.ID = pLoginPacket->GetID();
		userInfo.PW = pLoginPacket->GetPW();
		userInfo.FriendList = friendListQuery.GetFriendList();

		// ������ �������� ��ϵ� �޼����� ������� ä����Ŷ ������
		std::vector<ChatMessage>* unreadChattingList = UserManager::GetInst()->GetUnreadMessageList(pLoginPacket->GetID());
		if (nullptr != unreadChattingList)
		{
			userInfo.UnreadMessage = std::move(*unreadChattingList);
		}

		// ��� ����
		LoginResultPacket resultPacket(RESULT_TYPE::SUCCESS, userInfo);
		_TCPSession->Send(&resultPacket);

		return;
	}

	// ��� ����
	LoginResultPacket resultPacket(RESULT_TYPE::FAIL);
	_TCPSession->Send(&resultPacket);
}

void DispatchJoinPacket(TCPSession* _TCPSession, std::unique_ptr<PacketBase> _joinPacket)
{
	std::unique_ptr<JoinPacket> pJoinPacket(static_cast<JoinPacket*>(_joinPacket.release()));

	// DB ó��
	JoinQuery joinQurey(pJoinPacket->GetJoinID(), pJoinPacket->GetJoinPW());
	if (DBManager::GetInst()->DoQueryAndSetResult(joinQurey) == true)
	{
		// ���� ���࿡ �����Ѱ��
		JoinResultPacket resultPacket(RESULT_TYPE::SUCCESS);
		_TCPSession->Send(&resultPacket);
	}
	else
	{
		JoinResultPacket resultPacket(RESULT_TYPE::FAIL);
		_TCPSession->Send(&resultPacket);
	}
}

void DispatchIDCheckPacket(TCPSession* _TCPSession, std::unique_ptr<PacketBase> _IDCheckPacket)
{
	std::unique_ptr<IDCheckPacket> pIDCheckPacket(static_cast<IDCheckPacket*>(_IDCheckPacket.release()));

	// DB���� Ȯ��
	FindIDQuery query(pIDCheckPacket->GetID());
	if (DBManager::GetInst()->DoQueryAndSetResult(query) == true && query.IsFindSuccess() == false)
	{
		// ID�� ���� IDã�⿡ ������ ��� ����ó��
		IDCheckResultPacket resultPacket(RESULT_TYPE::SUCCESS);
		_TCPSession->Send(&resultPacket);
	}
	else
	{
		// ���� ����
		IDCheckResultPacket resultPacket(RESULT_TYPE::FAIL);
		_TCPSession->Send(&resultPacket);
	}
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

