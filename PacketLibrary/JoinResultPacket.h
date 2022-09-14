#pragma once
#include "PacketBase.h"

class JoinResultPacket : public PacketBase
{
private: // member var
	RESULT_TYPE m_joinResult;

public: // default
	JoinResultPacket();
	JoinResultPacket(RESULT_TYPE _joinResult);
	~JoinResultPacket();


protected:
	virtual size_t GetContentPacketSize() override;

public:
	Serializer Serialize() override;
	void Deserialize(const Serializer& _serialize) override;

public:
	RESULT_TYPE GetJoinResult() { return m_joinResult; }
};

