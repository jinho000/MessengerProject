#pragma once

#include <iostream>
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <Windows.h>
#include <MSWSock.h>

#include <mysql/mysql.h>

#include <functional>
#include <thread>
#include <vector>
#include <queue>
#include <string>
#include <list>
#include <unordered_map>
#include <mutex>
#include <cassert>

#pragma comment (lib, "libmysql.lib")
#pragma comment (lib, "ws2_32") 
#pragma comment (lib,"mswsock.lib")  // acceptEx 라이브러리


#include <PacketLibrary/enum.h>
#include <PacketLibrary/struct.h>


#include "enum.h"
#include "struct.h"
#include "using.h"
#include "ServerHelper.h"

