#pragma once
#include "PacketBase.h"


class JoinPacket : public PacketBase
{
private: // member var
	std::string m_ID;

public: // default
	JoinPacket();
	~JoinPacket();

	JoinPacket(const JoinPacket& _other) = delete;
	JoinPacket(JoinPacket&& _other) = delete;
	JoinPacket& operator=(const JoinPacket& _other) = delete;
	JoinPacket& operator=(const JoinPacket&& _other) = delete;

private:

public: // member Func
};

