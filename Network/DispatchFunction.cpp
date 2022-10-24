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

	// 전달받은 패킷 확인
	std::string log = "LoginPacket\n";
	log += "LoginID: " + pLoginPacket->GetID() + "\n";
	log += "LoginPW: " + pLoginPacket->GetPW() + "\n";
	Logger::GetInst()->Log(log);


	// 이미 접속중인 유저인지 확인
	if (UserManager::GetInst()->HasUser(pLoginPacket->GetID()) == true)
	{
		LoginResultPacket resultPacket(RESULT_TYPE::FAIL);
		std::string log = "LoginResultPacket\n";
		log += "User ID: " + pLoginPacket->GetID() + "\n";
		log += "Login Fail - Already Connected User\n";
		Logger::GetInst()->Log(log);

		// 이미 접속중이면 실패처리
		_TCPSession->Send(resultPacket);
		return;
	}

	// 로그인 처리
	LoginQuery loginQuery(pLoginPacket->GetID(), pLoginPacket->GetPW());
	if (DBManager::GetInst()->DoQueryAndSetResult(loginQuery) == true 
		&& loginQuery.IsLoginSuccess() == true)
	{
		// 로그인 쿼리에 성공하고 로그인에 성공한 경우, 서버에 접속처리
		UserManager::GetInst()->AddUser(pLoginPacket->GetID(), _TCPSession);
		
		// 로그인에 성공하고 유저의 ID index를 가져온경우 유저리스트쿼리를 실패할 수 없음
		FriendListQuery friendListQuery(loginQuery.GetUserID());
		DBManager::GetInst()->DoQueryAndSetResult(friendListQuery);


		// 유저 정보 가져오기
		UserInfo userInfo;
		userInfo.ID = pLoginPacket->GetID();
		userInfo.PW = pLoginPacket->GetPW();
		userInfo.FriendList = friendListQuery.GetFriendList();

		// 접속한 유저에게 등록된 메세지가 있을경우 채팅패킷 보내기
		std::vector<ChatMessage>* unreadChattingList = UserManager::GetInst()->GetUnreadMessageList(pLoginPacket->GetID());
		if (nullptr != unreadChattingList)
		{
			userInfo.UnreadMessage = std::move(*unreadChattingList);
		}

		// 로그 출력
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
		
		// 결과 전송
		LoginResultPacket resultPacket(RESULT_TYPE::SUCCESS, userInfo);
		_TCPSession->Send(resultPacket);

		return;
	}

	log = "LoginResultPacket\n";
	log += "User ID: " + pLoginPacket->GetID() + "\n";
	log += "Login Fail\n";
	Logger::GetInst()->Log(log);

	// 결과 전송
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
	

	// DB 처리
	log = "JoinResultPacket\n";
	log += "Join ID: " + pJoinPacket->GetJoinID() + "\n";
	log += "Join PW: " + pJoinPacket->GetJoinPW() + "\n";
	
	JoinQuery joinQurey(pJoinPacket->GetJoinID(), pJoinPacket->GetJoinPW());
	if (DBManager::GetInst()->DoQueryAndSetResult(joinQurey) == true)
	{
		log += "Join Success\n";
		Logger::GetInst()->Log(log);

		// 쿼리 실행에 성공한경우
		JoinResultPacket resultPacket(RESULT_TYPE::SUCCESS);
		_TCPSession->Send(resultPacket);
	}
	else
	{
		log += "Join Fail\n";
		Logger::GetInst()->Log(log);

		// 조인 실패
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


	// DB에서 확인
	FindIDQuery query(pIDCheckPacket->GetID());
	if (DBManager::GetInst()->DoQueryAndSetResult(query) == true && query.IsFindSuccess() == false)
	{
		// ID가 없어 ID찾기에 실패한 경우 성공처리
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
		// 쿼리 실패
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


	// DB에 등록된 아이디인지 확인
	FindIDQuery findIDQuery(friendID);
	if (DBManager::GetInst()->DoQueryAndSetResult(findIDQuery) == true && findIDQuery.IsFindSuccess() == true)
	{
		// DB에 등록된 아이디인경우 친구리스트에 추가
		// AddFriendQuery는 실패할일이 없다고 가정
		
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
	
	// 유저가 없는경우, 유저가 접속하지 않은경우
	// 등록되지 않은 유저에게는 채팅을 보낼 수 없음
	// 유저의 메세지 저장
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

	// 유저가 접속하지 않은경우 채팅을 저장하지 않으므로 그냥 버림
	// 채팅내용을 저장시 가지고 있어야함
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