#pragma once

#include <iostream>
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <Windows.h>
#include <MSWSock.h>

#include <functional>
#include <thread>
#include <vector>
#include <queue>
#include <string>
#include <list>
#include <mutex>
#include <cassert>

#pragma comment (lib, "ws2_32") 
#pragma comment(lib,"mswsock.lib")  // acceptEx 라이브러리


#include "enum.h"
#include "using.h"
#include "struct.h"
#include "ServerHelper.h"
