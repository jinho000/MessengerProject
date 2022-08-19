#include "pch.h"
#include "TCPSessionPool.h"
#include "TCPSession.h"
#include "ConfigManager.h" 

TCPSessionPool::TCPSessionPool()
{
    // �����ʰ� ���� ������� �� ����Ǯ�� �����������
    // TCPSession�� ���� �� accept ȣ��
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
    // accpetEx ���ε� �����Ѱ��
    // IOCP work�����尡 ����Ǿ� accept���¿��� ������ �������;��Ѵ�
    // accept���̾��� TCP������ ���⼭ ����
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
    // Ŭ���̾�Ʈ�� ������ ������ �� �ٽ� ��Ȱ���ϴ� TCP���Ǹ� �ִ´�
    assert(_pTCPSession->IsRecycleSession() == true);

    // ��Ȱ��� ������ �ٽ� accept ȣ�� �� ����Ǯ�� �ִ´�
    _pTCPSession->RequestAsyncAccept();

    m_lock.lock();
    m_TCPSessionPool.insert({ _pTCPSession, _pTCPSession });
    m_lock.unlock();
}
