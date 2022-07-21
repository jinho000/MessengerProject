#pragma once
#include "PacketBase.h"

class SendTestPacket : public PacketBase
{
private: // member var
	int m_sendData;

public: // default
	SendTestPacket();
	~SendTestPacket();

public:
	void Serialize() override;
	void Deserialize() override;
};

