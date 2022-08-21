#pragma once
#include "PacketBase.h"

class IDCheckResultPacket : public PacketBase
{
private: // member var
	RESULT_TYPE m_IDCheckResult;

public: // default
	IDCheckResultPacket();
	IDCheckResultPacket(RESULT_TYPE _IDCheckResultPacket);
	~IDCheckResultPacket();

protected:
	virtual size_t GetContentPacketSize() override;

public:
	void Serialize(Serializer& _serializer) override;
	void Deserialize(const Serializer& _serializer) override;

public:
	RESULT_TYPE GetIDCheckResult() { return m_IDCheckResult; }

};

