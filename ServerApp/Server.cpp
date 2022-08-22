#include "Server.h"
#include <Network/pch.h>
#include <Network/IOCP.h>
#include <Network/SessionManager.h>
#include <Network/ConfigManager.h>
#include <Network/ListenSocket.h>
#include <Network/TCPListener.h>
#include <Network/TCPSessionPool.h>
#include <Network/PacketHandler.h>
#include <Network/UserManager.h>

void Server::StartServer()
{
    ServerHelper::WSAStart();

    // ������ config ���� �� �ٸ� �Ŵ��� ����
    ConfigManager::CreateInstance();
    ConfigManager::GetInst()->LoadConfig();

    IOCP::CreateInstance();
    TCPListener::CreateInstance();
    SessionManager::CreateInstance();
    PacketHandler::CreateInstance();

    UserManager::CreateInstance();

    {
        // ���� ���� ����
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

    UserManager::Destroy();

    PacketHandler::Destroy();
    TCPSessionPool::Destroy();
    SessionManager::Destroy();
    TCPListener::Destroy();

    ConfigManager::Destroy();

    ServerHelper::WSAEnd();
}