#pragma once
#include "PacketBase.h"

class LoginPacket : public PacketBase
{
private: // member var
	std::string m_ID;
	std::string m_password;

public: // default
	LoginPacket(const std::string& _ID, const std::string& _password);
	~LoginPacket();

protected:
	virtual size_t GetContentPacketSize();

public:
	void Serialize(Serializer& _serialize) override;
	void Deserialize(const Serializer& _serialize) override;
};

