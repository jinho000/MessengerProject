#include "pch.h"
#include "UserManager.h"

UserManager::UserManager()
{
}

UserManager::~UserManager()
{
}

void UserManager::AddUser(const std::string& _userID, TCPSession* _pTCPSession)
{
    m_connectedUserTableLock.lock();
    m_connectedUserTable.insert(std::make_pair(_userID, _pTCPSession));
    m_connectedUserTableLock.unlock();
}

TCPSession* UserManager::FindUser(const std::string& _userID)
{
    auto iter = m_connectedUserTable.find(_userID);
    if (iter == m_connectedUserTable.end())
    {
        return nullptr;
    }

    return iter->second;
}
