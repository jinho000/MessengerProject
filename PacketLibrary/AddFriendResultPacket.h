#pragma once
#include "PacketBase.h"

class AddFriendResultPacket : public PacketBase
{
private: // member var
	std::string m_friendID;
	RESULT_TYPE m_addFriendResult;

public: // default
	AddFriendResultPacket();
	AddFriendResultPacket(const std::string& _friendID, RESULT_TYPE _addFriendResult);
	~AddFriendResultPacket();

protected:
	virtual size_t GetContentPacketSize() override;

public:
	Serializer Serialize() override;
	void Deserialize(const Serializer& _serializer) override;

public:
	RESULT_TYPE GetAddFriendResult() { return m_addFriendResult; }
	const std::string& GetFriendID() { return m_friendID; }
};

