#include "pch.h"
#include "Server.h"

void Server::StartServer()
{
    ServerHelper::WSAStart();



    ServerHelper::WSAEnd();
}
