#include "pch.h"
#include "TCPSessionPool.h"
#include "TCPSession.h"

TCPSessionPool::TCPSessionPool()
{
    CreateTCPSession(INITIAL_SESSION_COUNT);
}

TCPSessionPool::~TCPSessionPool()
{
    while (m_TCPSessionPool.empty() == false)
    {
        // delete를 호출하는 스레드는 하나뿐이므로 락을걸지 않는다
        TCPSession* pTCPSession = m_TCPSessionPool.front(); m_TCPSessionPool.pop();
        delete pTCPSession;
    }
}

void TCPSessionPool::CreateTCPSession(int _TCPSessionCount)
{
    m_lock.lock();
    for (size_t i = 0; i < _TCPSessionCount; i++)
    {
        TCPSession* pTCPSession = new TCPSession;
        m_TCPSessionPool.push(pTCPSession);
    }
    m_lock.unlock();
}

TCPSession* TCPSessionPool::GetTCPSession()
{
    if (m_TCPSessionPool.empty())
    {
        CreateTCPSession(RECREATE_SESSION_COUNT);
    }

    TCPSession* pTCPSession = m_TCPSessionPool.front();

    m_lock.lock();
    m_TCPSessionPool.pop();
    m_lock.unlock();

    return pTCPSession;
}

void TCPSessionPool::RetrieveTCPSession(TCPSession* _pTCPSession)
{
    // 클라이언트와 접속이 끊어진 후 다시 재활용하는 TCP세션만 넣는다
    assert(_pTCPSession->IsRecycleSession() == true);

    // accpet 요청 후 세션 풀에 넣는다
    m_lock.lock();
    m_TCPSessionPool.push(_pTCPSession);
    m_lock.unlock();
}
