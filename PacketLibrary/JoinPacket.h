#pragma once
#include "PacketBase.h"

class JoinPacket : public PacketBase
{
private: // member var
	std::string m_joinID;
	std::string m_joinPW;

public: // default
	JoinPacket();
	JoinPacket(const std::string& _id, const std::string& _pw);
	~JoinPacket();

protected:
	virtual size_t GetContentPacketSize() override;

public:
	void Serialize(Serializer& _serialize) override;
	void Deserialize(const Serializer& _serialize) override;

public:
	const std::string& GetJoinID() { return m_joinID; }
	const std::string& GetJoinPW() { return m_joinPW; }
};

