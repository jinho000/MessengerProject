#pragma once
#include "PacketBase.h"

class AddFriendPacket : public PacketBase
{
private: // member var
	std::string m_friendID;

public: // default
	AddFriendPacket();
	AddFriendPacket(const std::string& _friendID);
	~AddFriendPacket();

protected:
	virtual size_t GetContentPacketSize() override;

public:
	void Serialize(Serializer& _serialize) override;
	void Deserialize(const Serializer& _serialize) override;

public:
	const std::string& GetFriendID() { return m_friendID; }
};

