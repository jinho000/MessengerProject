#pragma once
#include "Singleton.h"
#include <PacketLibrary/ChattingPacket.h>

class UserManager : public Singleton<UserManager>
{
	friend class Singleton;
	using UnreadChattingList = std::queue<std::unique_ptr<ChattingPacket>>;

private: // member var
	std::unordered_map<std::string, TCPSession*>	m_connectedUserTable;
	std::mutex										m_connectedUserTableLock;


	std::unordered_map<std::string, UnreadChattingList>	m_unreadMessageTable;
	std::mutex											m_unreadMessageTableLock;


private: // default
	UserManager();
	~UserManager();

public: // member Func
	void AddUser(const std::string& _userID, TCPSession* _pTCPSession);
	void DeleteUser(const std::string& _userID);

	void AddUnreadChatting(const std::string& _recvUserID, std::unique_ptr<ChattingPacket> _sendPacket);
	void SendUnreadChatting(const std::string& _loginUserID, TCPSession* _loginUserSession);

	TCPSession* FindUser(const std::string& _userID);

};

