#pragma once
#include "PacketBase.h"

class AddFriendPacket : public PacketBase
{
private: // member var
	std::string m_userID;
	std::string m_friendID;

public: // default
	AddFriendPacket();
	AddFriendPacket(const std::string& _userID, const std::string& _friendID);
	~AddFriendPacket();

protected:
	virtual size_t GetContentPacketSize() override;

public:
	Serializer Serialize() override;
	void Deserialize(const Serializer& _serialize) override;

public:
	const std::string& GetFriendID() { return m_friendID; }
	const std::string& GetUserID() { return m_userID; }
};

