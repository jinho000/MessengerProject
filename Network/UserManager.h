#pragma once
#include "Singleton.h"

class UserManager : public Singleton<UserManager>
{
	friend class Singleton;

private: // member var
	std::unordered_map<std::string, TCPSession*>	m_connectedUserTable;
	std::mutex										m_connectedUserTableLock;

private: // default
	UserManager();
	~UserManager();

public: // member Func
	void AddUser(const std::string& _userID, TCPSession* _pTCPSession);
	TCPSession* FindUser(const std::string& _userID);

};

