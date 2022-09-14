#pragma once
#include "PacketBase.h"

class LoginResultPacket : public PacketBase
{
private: // member var
	RESULT_TYPE m_result;
	UserInfo	m_userInfo;

public: // default
	LoginResultPacket();
	LoginResultPacket(RESULT_TYPE _result);
	LoginResultPacket(RESULT_TYPE _result, UserInfo& _userInfo);
	~LoginResultPacket();


protected:
	virtual size_t GetContentPacketSize() override;

public:
	Serializer Serialize() override;
	void Deserialize(const Serializer& _serializer) override;

public:
	RESULT_TYPE GetLoginResult() { return m_result; }
	const UserInfo& GetUserInfo() { return m_userInfo; }
};

