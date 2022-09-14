#pragma once
#include "PacketBase.h"

class LoginPacket : public PacketBase
{
private: // member var
	std::string m_ID;
	std::string m_password;

public: // default
	LoginPacket();
	LoginPacket(const std::string& _ID, const std::string& _password);
	~LoginPacket();

protected:
	virtual size_t GetContentPacketSize() override;

public:
	Serializer Serialize() override;
	void Deserialize(const Serializer& _serialize) override;

public:
	const std::string& GetID() { return m_ID; }
	const std::string& GetPW() { return m_password; }

};

