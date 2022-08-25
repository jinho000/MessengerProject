#include "pch.h"
#include "UserManager.h"
#include "TCPSession.h"

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

void UserManager::DeleteUser(const std::string& _userID)
{
    m_connectedUserTableLock.lock();
    m_connectedUserTable.erase(_userID);
    m_connectedUserTableLock.unlock();
}

void UserManager::AddUnreadChatting(const std::string& _recvUserID, std::unique_ptr<ChattingPacket> _sendPacket)
{
    m_unreadMessageTableLock.lock();
    auto iter = m_unreadMessageTable.find(_recvUserID);
    if (iter == m_unreadMessageTable.end())
    {
        m_unreadMessageTable[_recvUserID] = UnreadChattingList();
        m_unreadMessageTable[_recvUserID].push_back(_sendPacket->GetChattingMessage());
    }
    else
    {
        iter->second.push_back(_sendPacket->GetChattingMessage());
    }
    m_unreadMessageTableLock.unlock();
}

std::vector<ChatMessage>* UserManager::GetUnreadMessageList(const std::string& _loginUserID_)
{
    auto iter = m_unreadMessageTable.find(_loginUserID_);
    if (iter == m_unreadMessageTable.end())
        return nullptr;
    return &iter->second;
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
