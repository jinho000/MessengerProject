#include "pch.h"
#include "Server.h"
#include "IOCP.h"
#include "SessionSocketPool.h"
#include "SessionManager.h"
#include "ListenSocket.h"

// 소켓정보 구조체
struct SOCKETINFO
{
    OVERLAPPED	overlapped;
    SOCKET		socket;
    char		buffer[255];
    int			bufferSize;
    WSABUF		wsabuf;
};

void IOCallback(DWORD transferredBytes, LPOVERLAPPED IOData)
{
    SOCKETINFO* clientInfo = reinterpret_cast<SOCKETINFO*>(IOData);
    clientInfo->buffer[transferredBytes] = '\0';
    std::string recvData(clientInfo->buffer);

    std::cout << recvData << std::endl;


    // 비동기 입출력 시작
    DWORD recvbytes = 0;
    DWORD flags = 0;
    int result = WSARecv(clientInfo->socket, &clientInfo->wsabuf, 1, &recvbytes, &flags, &clientInfo->overlapped, NULL);
    if (result == SOCKET_ERROR)
    {
        if (WSAGetLastError() == ERROR_IO_PENDING)
        {
            return;
        }

        assert(nullptr);
    }
}

IOCompletionCallback ioCallback = IOCallback;
const int SERVER_PORT = 9900;

//void Server::StartServer()
//{
//    ServerHelper::WSAStart();
//    IOCP::CreateInstance(10);
//
//    ListenSocket listenSock(9900, "127.0.0.1");
//    listenSock.StartListen();
//
//    // accpet
//    while (true)
//    {
//        SOCKADDR_IN clientAddr = {};
//        int len = sizeof(clientAddr);
//        SOCKET sessionSocket = accept(listenSock.GetSocket(), (sockaddr*)&clientAddr, &len);
//
//        // IOCP에 소켓 연결
//        IOCP::GetInst()->RegisterSocket(sessionSocket, &ioCallback);
//
//        // 클라이언트의 접속 및 정보 출력
//        char buff[255];
//        inet_ntop(AF_INET, &clientAddr.sin_addr, buff, sizeof(buff));
//        std::string ip(buff);
//        std::string portNum(std::to_string(ntohs(clientAddr.sin_port)));
//        std::cout << "IP " << ip << " Port: " << portNum << std::endl;
//
//
//        // 소켓 정보 구조체 할당
//        SOCKETINFO* clientInfo = new SOCKETINFO;
//        clientInfo->socket = sessionSocket;
//        clientInfo->bufferSize = 255;
//        clientInfo->wsabuf.buf = clientInfo->buffer;
//        clientInfo->wsabuf.len = clientInfo->bufferSize;
//        ZeroMemory(&clientInfo->overlapped, sizeof(clientInfo->overlapped));
//
//
//        // 비동기 입출력 시작
//        DWORD recvbytes = 0;
//        DWORD flags = 0;
//        int result = WSARecv(clientInfo->socket, &clientInfo->wsabuf, 1, &recvbytes, &flags, &clientInfo->overlapped, NULL);
//        if (result == SOCKET_ERROR)
//        {
//            if (WSAGetLastError() == ERROR_IO_PENDING)
//            {
//                continue;
//            }
//
//            assert(nullptr);
//        }
//    }
//
//
//    IOCP::Destroy();
//    ServerHelper::WSAEnd();
//}


void Server::StartServer()
{
    ServerHelper::WSAStart();

    // CPU스레드 개수 * 2 - 1만큼 스레드 만들기
    // 1개는 메인 스레드
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    int workerThreadCount = sysinfo.dwNumberOfProcessors * 2 - 1;

    IOCP::CreateInstance(1);
    SessionSocketPool::CreateInstance(1);
    SessionManager::CreateInstance();

    {
        // 리슨 서버 시작
        ListenSocket listenSocket(SERVER_PORT, "127.0.0.1");
        listenSocket.StartListen();

        while (true)
        {
            std::string wait;
            std::cin >> wait;

            if (wait == "quit")
            {
                break;
            }
        }

        // 이 블록을 나갈때 리슨소켓을 닫음
    }
    
    SessionManager::Destroy();
    SessionSocketPool::Destroy();
    IOCP::Destroy();
    ServerHelper::WSAEnd();
}