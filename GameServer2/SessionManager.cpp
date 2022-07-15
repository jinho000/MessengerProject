#include "pch.h"
#include "SessionManager.h"
#include "SessionSocket.h"

SessionManager* SessionManager::pInstance = nullptr;

SessionManager::SessionManager()
{
}

SessionManager::~SessionManager()
{
    while (m_socketList.empty() == false)
    {
        // delete를 호출하는 스레드는 하나뿐이므로 락을걸지 않는다
        SessionSocket* pSessionSocket = m_socketList.front(); m_socketList.pop_front();
        delete pSessionSocket;
    }
}

void SessionManager::CreateInstance()
{
    if (pInstance != nullptr)
        return;

    pInstance = new SessionManager;
}

void SessionManager::Destroy()
{
    if (pInstance != nullptr)
    {
        // 삭제


        delete pInstance;
        pInstance = nullptr;
    }
}

SessionManager* SessionManager::GetInst()
{
    return pInstance;
}

void SessionManager::AddSessionSocket(SessionSocket* _pSessionSocket)
{
    m_lock.lock();
    m_socketList.push_back(_pSessionSocket);
    m_lock.unlock();
}
