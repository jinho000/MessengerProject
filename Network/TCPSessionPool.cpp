#include "pch.h"
#include "TCPSessionPool.h"
#include "TCPSession.h"
#include "ConfigManager.h" 

TCPSessionPool::TCPSessionPool()
    : m_TCPSessionObjPool(ConfigManager::GetInst()->GetMaxConnection())
{
    // 리스너가 먼저 만들어진 후 세션풀이 만들어져야함
    // TCPSession을 생성 후 accept 호출
    int maxConnection = ConfigManager::GetInst()->GetMaxConnection();
    for (size_t i = 0; i < maxConnection; i++)
    {
        TCPSession* pTCPSession = new TCPSession;
        pTCPSession->RequestAsyncAccept();
        m_TCPSessionPool.insert({ pTCPSession, pTCPSession });
    }
}

TCPSessionPool::~TCPSessionPool()
{
    // accpetEx 중인데 종료한경우
    // IOCP work스레드가 종료되어 accept상태에서 소켓이 빠져나와야한다
    // accept중이었던 TCP세션을 여기서 종료
    auto iter = m_TCPSessionPool.begin();
    while (iter != m_TCPSessionPool.end())
    {
        delete iter->second;
        iter->second = nullptr;
        ++iter;
    }

    m_TCPSessionPool.clear();
}

void TCPSessionPool::PopTCPSession(TCPSession* _pTCPSession)
{
    m_lock.lock();
    m_TCPSessionPool.erase(_pTCPSession);
    m_lock.unlock();
}

void TCPSessionPool::RetrieveTCPSession(TCPSession* _pTCPSession)
{
    // 클라이언트와 접속이 끊어진 후 다시 재활용하는 TCP세션만 넣는다
    assert(_pTCPSession->IsRecycleSession() == true);

    // 재활용된 세션을 다시 accept 호출 후 세션풀에 넣는다
    _pTCPSession->RequestAsyncAccept();

    m_lock.lock();
    m_TCPSessionPool.insert({ _pTCPSession, _pTCPSession });
    m_lock.unlock();
}
