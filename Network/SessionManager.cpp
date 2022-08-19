#include "pch.h"
#include "SessionManager.h"
#include "TCPSession.h"

SessionManager::SessionManager()
{
}

SessionManager::~SessionManager()
{
    // ������ Ŭ���̾�Ʈ�� ����Ǿ��ִµ� �����Ѱ��?
    // ���� ���� ó��

    // delete�� ȣ���ϴ� ������� �ϳ����̹Ƿ� �������� �ʴ´�
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
