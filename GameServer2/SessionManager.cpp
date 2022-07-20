#include "pch.h"
#include "SessionManager.h"
#include "TCPSession.h"

SessionManager::SessionManager()
{
}

SessionManager::~SessionManager()
{
    // 소켓이 클라이언트와 연결되어있는데 종료한경우?
    while (m_connectedTCPSession.empty() == false)
    {
        // delete를 호출하는 스레드는 하나뿐이므로 락을걸지 않는다
        TCPSession* pTCPSession = m_connectedTCPSession.front(); m_connectedTCPSession.pop_front();
        delete pTCPSession;
        pTCPSession = nullptr;
    }
}

void SessionManager::AddTCPSession(TCPSession* _pTCPSession)
{
    m_lock.lock();
    m_connectedTCPSession.push_back(_pTCPSession);
    m_lock.unlock();
}

