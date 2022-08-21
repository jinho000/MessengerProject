#pragma once
#include "PacketBase.h"

class AddFriendResultPacket : public PacketBase
{
private: // member var
	RESULT_TYPE m_addFriendResult;

public: // default
	AddFriendResultPacket();
	AddFriendResultPacket(RESULT_TYPE _addFriendResult);
	~AddFriendResultPacket();

protected:
	virtual size_t GetContentPacketSize() override;

public:
	void Serialize(Serializer& _serializer) override;
	void Deserialize(const Serializer& _serializer) override;

public:
	RESULT_TYPE GetAddFriendResult() { return m_addFriendResult; }
};

