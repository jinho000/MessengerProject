#include "Server.h"
#include <filesystem>
#include <Network/pch.h>
#include <Network/IOCP.h>
#include <Network/SessionManager.h>
#include <Network/ConfigManager.h>
#include <Network/ListenSocket.h>
#include <Network/TCPListener.h>
#include <Network/TCPSessionPool.h>
#include <Network/PacketHandler.h>
#include <Network/UserManager.h>
#include <Network/DBManager.h>
#include <Network/Logger.h>
#include "tinyxml2.h"

void Server::StartServer()
{
    ServerHelper::WSAStart();

    // 서버의 config 설정 후 다른 매니저 생성
    ConfigManager::CreateInstance();
    if (SetServerConfig() == false)
    {
        // 설정파일 로딩 실패시 종료
        ConfigManager::Destroy();
        ServerHelper::WSAEnd();
        system("pause");

        return;
    }

    IOCP::CreateInstance();
    TCPListener::CreateInstance();
    SessionManager::CreateInstance();
    PacketHandler::CreateInstance();

    UserManager::CreateInstance();
    DBManager::CreateInstance();

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

    DBManager::Destroy();
    UserManager::Destroy();

    PacketHandler::Destroy();
    TCPSessionPool::Destroy();
    SessionManager::Destroy();
    TCPListener::Destroy();

    ConfigManager::Destroy();

    ServerHelper::WSAEnd();

    mysql_library_end();
}

bool Server::SetServerConfig()
{
    //std::string currPath = std::filesystem::current_path().string();
    //std::cout << currPath << std::endl;

    std::string log;
    tinyxml2::XMLDocument doc;
    doc.LoadFile("ServerConfig.xml");
    if (doc.Error())
    {
        std::cout << "configfile load error" << std::endl;
        return false;
    }

    // 시작 루트 경로
    tinyxml2::XMLElement* Root = doc.FirstChildElement("ServerApp");
    if (nullptr == Root)
    {
        std::cout << "ServerApp element error" << std::endl;
        return false;
    }

    
    // 서버정보 세팅
    {
        tinyxml2::XMLElement* ServerStart = Root->FirstChildElement("ServerStart");
        int serverPort = nullptr != ServerStart->FindAttribute("Port") ? std::stoi(ServerStart->FindAttribute("Port")->Value()) : -1;
        int maxConnection = nullptr != ServerStart->FindAttribute("MaxConnection") ? std::stoi(ServerStart->FindAttribute("MaxConnection")->Value()) : -1;
        std::string serverIP = nullptr != ServerStart->FindAttribute("ServerIP") ? ServerStart->FindAttribute("ServerIP")->Value() : "";
        int logicThreadCount = nullptr != ServerStart->FindAttribute("LogicThreadCount") ? std::stoi(ServerStart->FindAttribute("MaxConnection")->Value()) : -1;

        ConfigManager::GetInst()->SetLogicThreadCount(logicThreadCount);
        ConfigManager::GetInst()->SetServerInfo(serverIP, serverPort, maxConnection);

        log += "Server IP Address: " + serverIP + "\n";
        log += "Server Port: " + std::to_string(serverPort) + "\n";
    }


    // DB정보 세팅
    {
        tinyxml2::XMLElement* database = Root->FirstChildElement("DataBase");
        DBServerInfo dbInfo = {};
        dbInfo.Host = nullptr != database->FindAttribute("Host") ? database->FindAttribute("Host")->Value() : "";
        dbInfo.User = nullptr != database->FindAttribute("User")->Value() ? database->FindAttribute("User")->Value() : "";
        dbInfo.Password = nullptr != database->FindAttribute("PW")->Value() ? database->FindAttribute("PW")->Value() : "";
        dbInfo.Schema = nullptr != database->FindAttribute("Name")->Value() ? database->FindAttribute("Name")->Value() : "";
        dbInfo.Port = nullptr != database->FindAttribute("Port")->Value() ? std::stoi(database->FindAttribute("Port")->Value()) : -1;

        ConfigManager::GetInst()->SetDBServerInfo(dbInfo);

        log += "DB Host: " + dbInfo.Host + "\n";
        log += "DB User: " + dbInfo.User+ "\n";
        log += "DB PW: " + dbInfo.Password+ "\n";
        log += "DB Schema: " + dbInfo.Schema+ "\n";
        log += "DB Port: " + std::to_string(dbInfo.Port)+ "\n";
    }

    Logger::GetInst()->Log(log);

    return true;
}
