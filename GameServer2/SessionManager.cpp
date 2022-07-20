#include "pch.h"
#include "SessionManager.h"
#include "TCPSession.h"

SessionManager::SessionManager()
{
}

SessionManager::~SessionManager()
{
    // 소켓이 클라이언트와 연결되어있는데 종료한경우?
    // 소켓 종료 처리

    // delete를 호출하는 스레드는 하나뿐이므로 락을걸지 않는다
    auto iter = m_connectedTCPSession.begin();
    while (iter != m_connectedTCPSession.end())
    {
        delete iter->second;
        iter->second = nullptr;
        ++iter;
    }

    m_connectedTCPSession.clear();
}

void SessionManager::AddTCPSession(TCPSession* _pTCPSession)
{
    m_lock.lock();
    m_connectedTCPSession.insert({ _pTCPSession , _pTCPSession });
    m_lock.unlock();
}

void SessionManager::PopTCPSession(TCPSession* _key)
{
    m_lock.lock();
    m_connectedTCPSession.erase(_key);
    m_lock.unlock();
}
