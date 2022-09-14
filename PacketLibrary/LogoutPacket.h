#pragma once
#include "PacketBase.h"

class LogoutPacket : public PacketBase
{
private: // member var
	std::string m_logoutID;

public: // default
	LogoutPacket();
	LogoutPacket(const std::string& _logoutID);
	~LogoutPacket();
	
protected:
	virtual size_t GetContentPacketSize() override;

public:
	Serializer Serialize() override;
	void Deserialize(const Serializer& _serializer) override;

	const std::string& GetLogoutID() { return m_logoutID; }
};

