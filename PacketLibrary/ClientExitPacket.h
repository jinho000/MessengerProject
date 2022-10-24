#pragma once
#include "PacketBase.h"

class ClientExitPacket : public PacketBase
{
private: // member var
	bool m_bExit;

public: // default
	ClientExitPacket();
	~ClientExitPacket();

protected:
	virtual size_t GetContentPacketSize() override;

public:
	virtual Serializer Serialize() override;
	virtual void Deserialize(const Serializer& _serializer) override;

};

