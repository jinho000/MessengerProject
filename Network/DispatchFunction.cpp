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
#include "AddFriendQuery.h"
#include "Logger.h"

void DispatchLoginPacket(TCPSession* _TCPSession, std::unique_ptr<PacketBase> _loginPacket)
{
	std::unique_ptr<LoginPacket> pLoginPacket(static_cast<LoginPacket*>(_loginPacket.release()));

	// ���޹��� ��Ŷ Ȯ��
	std::string log = "LoginPacket\n";
	log += "LoginID: " + pLoginPacket->GetID() + "\n";
	log += "LoginPW: " + pLoginPacket->GetPW() + "\n";
	Logger::GetInst()->Log(log);


	// �̹� �������� �������� Ȯ��
	if (UserManager::GetInst()->HasUser(pLoginPacket->GetID()) == true)
	{
		LoginResultPacket resultPacket(RESULT_TYPE::FAIL);
		std::string log = "LoginResultPacket\n";
		log += "User ID: " + pLoginPacket->GetID() + "\n";
		log += "Login Fail - Already Connected User\n";
		Logger::GetInst()->Log(log);

		// �̹� �������̸� ����ó��
		_TCPSession->Send(resultPacket);
		return;
	}

	// �α��� ó��
	LoginQuery loginQuery(pLoginPacket->GetID(), pLoginPacket->GetPW());
	if (DBManager::GetInst()->DoQueryAndSetResult(loginQuery) == true 
		&& loginQuery.IsLoginSuccess() == true)
	{
		// �α��� ������ �����ϰ� �α��ο� ������ ���, ������ ����ó��
		UserManager::GetInst()->AddUser(pLoginPacket->GetID(), _TCPSession);
		
		// �α��ο� �����ϰ� ������ ID index�� �����°�� ��������Ʈ������ ������ �� ����
		FriendListQuery friendListQuery(loginQuery.GetUserID());
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

		// �α� ���
		std::string log = "LoginResultPacket\n";
		log += "User ID: " + userInfo.ID + "\n";
		log += "User PW: " + userInfo.PW + "\n";
		log += "User Friend List \n";
		for (size_t i = 0; i < userInfo.FriendList.size(); i++)
		{
			log += userInfo.FriendList[i];
			log += "\n";
		}
		log += "Login Success\n";
		Logger::GetInst()->Log(log);
		
		// ��� ����
		LoginResultPacket resultPacket(RESULT_TYPE::SUCCESS, userInfo);
		_TCPSession->Send(resultPacket);

		return;
	}

	log = "LoginResultPacket\n";
	log += "User ID: " + pLoginPacket->GetID() + "\n";
	log += "Login Fail\n";
	Logger::GetInst()->Log(log);

	// ��� ����
	LoginResultPacket resultPacket(RESULT_TYPE::FAIL);
	_TCPSession->Send(resultPacket);
}

void DispatchJoinPacket(TCPSession* _TCPSession, std::unique_ptr<PacketBase> _joinPacket)
{
	std::unique_ptr<JoinPacket> pJoinPacket(static_cast<JoinPacket*>(_joinPacket.release()));
	std::string log = "JoinPacket\n";
	log += "JoinID: " + pJoinPacket->GetJoinID() + "\n";
	log += "JoinPW: " + pJoinPacket->GetJoinPW() + "\n";
	Logger::GetInst()->Log(log);
	

	// DB ó��
	log = "JoinResultPacket\n";
	log += "Join ID: " + pJoinPacket->GetJoinID() + "\n";
	log += "Join PW: " + pJoinPacket->GetJoinPW() + "\n";
	
	JoinQuery joinQurey(pJoinPacket->GetJoinID(), pJoinPacket->GetJoinPW());
	if (DBManager::GetInst()->DoQueryAndSetResult(joinQurey) == true)
	{
		log += "Join Success\n";
		Logger::GetInst()->Log(log);

		// ���� ���࿡ �����Ѱ��
		JoinResultPacket resultPacket(RESULT_TYPE::SUCCESS);
		_TCPSession->Send(resultPacket);
	}
	else
	{
		log += "Join Fail\n";
		Logger::GetInst()->Log(log);

		// ���� ����
		JoinResultPacket resultPacket(RESULT_TYPE::FAIL);
		_TCPSession->Send(resultPacket);
	}
}

void DispatchIDCheckPacket(TCPSession* _TCPSession, std::unique_ptr<PacketBase> _IDCheckPacket)
{
	std::unique_ptr<IDCheckPacket> pIDCheckPacket(static_cast<IDCheckPacket*>(_IDCheckPacket.release()));
	std::string log = "IDCheckPacket\n";
	log += "CheckID: " + pIDCheckPacket->GetID() + "\n";
	Logger::GetInst()->Log(log);


	// DB���� Ȯ��
	FindIDQuery query(pIDCheckPacket->GetID());
	if (DBManager::GetInst()->DoQueryAndSetResult(query) == true && query.IsFindSuccess() == false)
	{
		// ID�� ���� IDã�⿡ ������ ��� ����ó��
		std::string log;
		log += "IDCheckResultPacket\n";
		log += "IDCheck Success\n";
		log += "Check ID: " + pIDCheckPacket->GetID() + "\n";
		Logger::GetInst()->Log(log);

		IDCheckResultPacket resultPacket(RESULT_TYPE::SUCCESS);
		_TCPSession->Send(resultPacket);
	}
	else
	{
		// ���� ����
		std::string log;
		log += "IDCheckResultPacket\n";
		log += "IDCheck Fail - ID Already Exist\n";
		log += "Check ID: " + pIDCheckPacket->GetID() + "\n";
		Logger::GetInst()->Log(log);

		IDCheckResultPacket resultPacket(RESULT_TYPE::FAIL);
		_TCPSession->Send(resultPacket);
	}
}

void DispatchAddFriendPacket(TCPSession* _TCPSession, std::unique_ptr<PacketBase> _AddFriendPacket)
{
	std::unique_ptr<AddFriendPacket> pAddFriendPacket(static_cast<AddFriendPacket*>(_AddFriendPacket.release()));
	const std::string& friendID = pAddFriendPacket->GetFriendID();

	std::string log = "AddFriendPacket\n";
	log += "Add Friend ID: " + friendID + "\n";
	Logger::GetInst()->Log(log);


	// DB�� ��ϵ� ���̵����� Ȯ��
	FindIDQuery findIDQuery(friendID);
	if (DBManager::GetInst()->DoQueryAndSetResult(findIDQuery) == true && findIDQuery.IsFindSuccess() == true)
	{
		// DB�� ��ϵ� ���̵��ΰ�� ģ������Ʈ�� �߰�
		// AddFriendQuery�� ���������� ���ٰ� ����
		
		AddFriendQuery addFriendQuery(pAddFriendPacket->GetUserID(), friendID);
		DBManager::GetInst()->DoQueryAndSetResult(addFriendQuery);

		std::string log;
		log += "AddFriendResultPacket\n";
		log += "Add friend Success\n";
		log += "Add friend ID: " + friendID + "\n";
		Logger::GetInst()->Log(log);

		AddFriendResultPacket resultPacket(friendID, RESULT_TYPE::SUCCESS);
		_TCPSession->Send(resultPacket);

		return;
	}

	log = "AddFriendResultPacket\n";
	log += "Add friend Fail - User Not Found \n";
	log += "Add friend ID: " + friendID + "\n";
	Logger::GetInst()->Log(log);

	AddFriendResultPacket resultPacket(friendID, RESULT_TYPE::FAIL);
	_TCPSession->Send(resultPacket);
}

void DispatchSendChattingPacket(TCPSession* _TCPSession, std::unique_ptr<PacketBase> _sendChattingPacket)
{
	std::unique_ptr<ChattingPacket> pSendChattingPacket(static_cast<ChattingPacket*>(_sendChattingPacket.release()));
	const ChatMessage& chatMessage = pSendChattingPacket->GetChattingMessage();

	std::string log = "ChattingPacket\n";
	log += "Send ID: " + chatMessage.sendUserID + "\n";
	log += "Recv ID: " + chatMessage.recvUserID + "\n";
	log += "Message: " + chatMessage.message + "\n";
	Logger::GetInst()->Log(log);


	TCPSession* pRecvUserSession = UserManager::GetInst()->FindUser(pSendChattingPacket->GetChattingMessage().recvUserID);
	
	// ������ ���°��, ������ �������� �������
	// ��ϵ��� ���� �������Դ� ä���� ���� �� ����
	// ������ �޼��� ����
	if (nullptr == pRecvUserSession)
	{
		Logger::GetInst()->Log("User Not Found - Save SendPacket\n");
		std::string recvUserID = pSendChattingPacket->GetChattingMessage().recvUserID;
		UserManager::GetInst()->AddUnreadChatting(recvUserID, std::move(pSendChattingPacket));
		return;
	}

	ChattingPacket recvPacket(pSendChattingPacket->GetChattingMessage());
	pRecvUserSession->Send(recvPacket);
}

void DispatchLogoutPacket(TCPSession* _TCPSession, std::unique_ptr<PacketBase> _logoutPacket)
{
	std::unique_ptr<LogoutPacket> pLogoutPacket(static_cast<LogoutPacket*>(_logoutPacket.release()));
	
	std::string log;
	log += "LogoutPacket\n";
	log += "Logout User ID: " + pLogoutPacket->GetLogoutID() + "\n";
	Logger::GetInst()->Log(log);
	
	UserManager::GetInst()->DeleteUser(pLogoutPacket->GetLogoutID());
}

void DispatchReadChattingPacket(TCPSession* _TCPSession, std::unique_ptr<PacketBase> _readChattingPacket)
{
	std::unique_ptr<ReadChattingPacket> pReadChattingPacket(static_cast<ReadChattingPacket*>(_readChattingPacket.release()));

	std::string log = "ReadChattingPacket\n";
	log += "SendUser ID: " + pReadChattingPacket->GetSendUserID() + "\n";
	log += "RecvUser ID: " + pReadChattingPacket->GetRecvUserID() + "\n";
	log += "ReadCount: " + std::to_string(pReadChattingPacket->GetReadCount()) + '\n';
	Logger::GetInst()->Log(log);

	TCPSession* pRecvUserSession = UserManager::GetInst()->FindUser(pReadChattingPacket->GetSendUserID());

	// ������ �������� ������� ä���� �������� �����Ƿ� �׳� ����
	// ä�ó����� ����� ������ �־����
	if (nullptr == pRecvUserSession)
	{
		Logger::GetInst()->Log("User Not Found\n");
		return;
	}

	pRecvUserSession->Send(*pReadChattingPacket);
}

void DispatchClientExitPacket(TCPSession* _TCPSession, std::unique_ptr<PacketBase> _clientExitPacket)
{
	Sleep(1);
	_TCPSession->Send(*_clientExitPacket);
}