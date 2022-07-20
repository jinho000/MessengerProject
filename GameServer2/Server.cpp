#include "pch.h"
#include "Server.h"
#include "IOCP.h"
#include "SessionManager.h"
#include "ConfigManager.h"
#include "ListenSocket.h"
#include "TCPListener.h"
#include "TCPSessionPool.h"

void Server::StartServer()
{
    ServerHelper::WSAStart();

    IOCP::CreateInstance();
    SessionManager::CreateInstance();
    TCPSessionPool::CreateInstance();

    // config 설정 후 TCPListener 생성
    ConfigManager::CreateInstance();
    ConfigManager::GetInst()->LoadConfig();

    TCPListener::CreateInstance();

    {
        // 리슨 서버 시작
        TCPListener::GetInst()->StartListen();

        while (true)
        {
            std::string wait;
            std::cin >> wait;

            if (wait == "quit")
            {
                break;
            }
        }
    }
    
    IOCP::Destroy();

    TCPSessionPool::Destroy();
    TCPListener::Destroy();
    SessionManager::Destroy();

    ConfigManager::Destroy();


    ServerHelper::WSAEnd();
}