#pragma once

namespace ServerHelper
{
	void WSAStart();
	void WSAEnd();

	void PrintLastError(const char* headMsg);
	void Log(std::string log);
}