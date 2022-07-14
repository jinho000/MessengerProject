#include "pch.h"
#include "Server.h"

using namespace std;


int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    Server::StartServer();

    return 0;
}

