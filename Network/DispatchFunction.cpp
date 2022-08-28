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

	// 로그인 처리
	LoginQuery loginQuery(pLoginPacket->GetID(), pLoginPacket->GetPW());
	if (DBManager::GetInst()->DoQueryAndSetResult(loginQuery) == true 
		&& loginQuery.IsLoginSuccess() == true)
	{
		// 로그인 쿼리에 성공하고 로그인에 성공한 경우, 서버에 접속처리
		UserManager::GetInst()->AddUser(pLoginPacket->GetID(), _TCPSession);
		
		// 로그인에 성공하고 유저의 ID index를 가져온경우 유저리스트쿼리를 실패할 수 없음
		int userIDIndex = loginQuery.GetUserIDIndex();
		FriendListQuery friendListQuery(userIDIndex);
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

		// 결과 전송
		LoginResultPacket resultPacket(RESULT_TYPE::SUCCESS, userInfo);
		_TCPSession->Send(&resultPacket);

		return;
	}

	// 결과 전송
	LoginResultPacket resultPacket(RESULT_TYPE::FAIL);
	_TCPSession->Send(&resultPacket);
}

void DispatchJoinPacket(TCPSession* _TCPSession, std::unique_ptr<PacketBase> _joinPacket)
{
	std::unique_ptr<JoinPacket> pJoinPacket(static_cast<JoinPacket*>(_joinPacket.release()));

	// DB 처리
	JoinQuery joinQurey(pJoinPacket->GetJoinID(), pJoinPacket->GetJoinPW());
	if (DBManager::GetInst()->DoQueryAndSetResult(joinQurey) == true)
	{
		// 쿼리 실행에 성공한경우
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

	// DB에서 확인
	FindIDQuery query(pIDCheckPacket->GetID());
	if (DBManager::GetInst()->DoQueryAndSetResult(query) == true && query.IsFindSuccess() == false)
	{
		// ID가 없어 ID찾기에 실패한 경우 성공처리
		IDCheckResultPacket resultPacket(RESULT_TYPE::SUCCESS);
		_TCPSession->Send(&resultPacket);
	}
	else
	{
		// 쿼리 실패
		IDCheckResultPacket resultPacket(RESULT_TYPE::FAIL);
		_TCPSession->Send(&resultPacket);
	}
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
	std::unique_ptr<ChattingPacket> pSendChattingPacket(static_cast<ChattingPacket*>(_sendChattingPacket.release()));

	TCPSession* pRecvUserSession = UserManager::GetInst()->FindUser(pSendChattingPacket->GetChattingMessage().recvUserID);
	
	// 유저가 없는경우, 유저가 접속하지 않은경우
	// 등록되지 않은 유저에게는 채팅을 보낼 수 없음
	// 유저의 메세지 저장
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

	// 유저가 접속하지 않은경우 채팅을 저장하지 않으므로 그냥 버림
	// 채팅내용을 저장시 가지고 있어야함
	if (nullptr == pRecvUserSession)
	{
		return;
	}

	pRecvUserSession->Send(pReadChattingPacket.get());
}

