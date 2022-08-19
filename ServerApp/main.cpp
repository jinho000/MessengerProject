#include <iostream>
#include "Server.h"

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(250);

    Server::StartServer();


    return EXIT_SUCCESS;
}
