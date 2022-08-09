#pragma once
#include <PacketLibrary/LoginPacket.h>

class TCPSession;
void LoginPacketHandler(TCPSession* _TCPSession, PacketBase* _loginPacket)
{
	LoginPacket* pLoginPacket = static_cast<LoginPacket*>(_loginPacket);
	
	int a = 0;


	delete pLoginPacket;
}