#pragma once
#include "PacketBase.h"

class IDCheckPacket : public PacketBase
{
private: // member var
	std::string m_ID;

public: // default
	IDCheckPacket();
	IDCheckPacket(const std::string& _ID);
	~IDCheckPacket();


protected:
	virtual size_t GetContentPacketSize() override;

public:
	Serializer Serialize() override;
	void Deserialize(const Serializer& _serialize) override;

public:
	const std::string& GetID() { return m_ID; }
};

