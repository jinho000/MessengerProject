#include "Server.h"
#include <Network/pch.h>
#include <Network/IOCP.h>
#include <Network/SessionManager.h>
#include <Network/ConfigManager.h>
#include <Network/ListenSocket.h>
#include <Network/TCPListener.h>
#include <Network/TCPSessionPool.h>
#include <Network/PacketHandler.h>

void Server::StartServer()
{
    ServerHelper::WSAStart();

    // 서버의 config 설정 후 다른 매니저 생성
    ConfigManager::CreateInstance();
    ConfigManager::GetInst()->LoadConfig();

    IOCP::CreateInstance();
    TCPListener::CreateInstance();
    SessionManager::CreateInstance();
    PacketHandler::CreateInstance();

    {
        // 리슨 서버 시작
        TCPListener::GetInst()->StartListen();
        TCPSessionPool::CreateInstance();

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

    PacketHandler::Destroy();
    TCPSessionPool::Destroy();
    SessionManager::Destroy();
    TCPListener::Destroy();

    ConfigManager::Destroy();

    ServerHelper::WSAEnd();
}