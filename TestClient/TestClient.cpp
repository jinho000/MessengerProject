#include <iostream>
#include <GameServer2/pch.h>
#include <GameServer2/ClientSocket.h>

ClientSocket* clientSocket = nullptr;

void WorkThread()
{

    while (true)
    {
        char buffer[255] = {};
        int len = 255;
        
        int result = recv(clientSocket->GetSocket(),  buffer, len, 0);
        if (result == 0)
        {
            break;
        }
        
        std::string recvStr(buffer);
        std::cout << recvStr << std::endl;
        
        if (recvStr == "quit")
        {
            break;
        }
    }
}

int main()
{
    ServerHelper::WSAStart();
    
    clientSocket = new ClientSocket(9900, "127.0.0.1", IPPROTO::IPPROTO_TCP);
    while (clientSocket->ConnectServer() == false)
    {
        std::cout << "서버 연결에 실패했습니다 서버에 다시 연결하려면 아무키나 입력" << std::endl;
        std::string wait;
        std::cin >> wait;

    }

    std::thread workerThread(WorkThread);

    while (true)
    {
        std::string input;
        std::cin >> input;

        send(clientSocket->GetSocket(), input.c_str(), input.length(), 0);

        if (input == "quit")
        {
            break;
        }
    }

    workerThread.join();

    return 0;
}
