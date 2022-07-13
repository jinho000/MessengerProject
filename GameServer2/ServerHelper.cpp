#include "pch.h"
#include "ServerHelper.h"

void ServerHelper::WSAStart()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa))
	{
		PrintLastError("WSAStartup Error");
	}
}

void ServerHelper::WSAEnd()
{
	WSACleanup();
}

void ServerHelper::PrintLastError(const char* headMsg)
{
	DWORD errorCode = WSAGetLastError();
	LPVOID msgBuff = nullptr;

	FormatMessageA(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		NULL,
		errorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&msgBuff,
		0,
		NULL);

	// print errorCode message 
	printf_s("[%s] Error Code : %d, Message : %s", headMsg, errorCode, (char*)msgBuff);
	LocalFree(msgBuff);
}

void ServerHelper::Log(std::string log)
{
	log += "\n";
	std::cout << log;
}
