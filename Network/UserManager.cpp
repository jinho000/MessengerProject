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
        //UnreadChattingList qu;
        //qu.push(std::move(_sendPacket));
        //m_unreadMessageTable.insert(std::make_pair(_recvUserID, std::move(qu)));

        m_unreadMessageTable[_recvUserID] = UnreadChattingList();
        m_unreadMessageTable[_recvUserID].push(std::move(_sendPacket));
    }
    else
    {
        iter->second.push(std::move(_sendPacket));
    }
    m_unreadMessageTableLock.unlock();
}

void UserManager::SendUnreadChatting(const std::string& _loginUserID, TCPSession* _loginUserSession)
{
    std::lock_guard<std::mutex> lock(m_unreadMessageTableLock);

    auto iter = m_unreadMessageTable.find(_loginUserID);
    
    // 읽지않은 채팅이 있을경우 전송하기
    if (iter != m_unreadMessageTable.end())
    {
        UnreadChattingList& unreadChatList = iter->second;
        
        while (unreadChatList.empty() == false)
        {
            ChattingPacket& sendPacket = *unreadChatList.front();
            ChattingPacket recvPacket(sendPacket.GetChattingMessage());
            _loginUserSession->Send(&recvPacket);
            unreadChatList.pop();
        }

        return;
    }

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
