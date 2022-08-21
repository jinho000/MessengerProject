#pragma once
#include "PacketBase.h"

class SendChattingPacket : public PacketBase
{
private: // member var
	std::string m_sendUserID;
	std::string m_recvUserID;
	std::string m_chatMessage;

public: // default
	SendChattingPacket();
	SendChattingPacket(const std::string& _sendUserID, const std::string& _recvUserID, const std::string& _chatMessage);
	~SendChattingPacket();

protected:
	virtual size_t GetContentPacketSize() override;

public:
	void Serialize(Serializer& _serializer) override;
	void Deserialize(const Serializer& _serializer) override;

public:
	const std::string& GetSendUserID() { return m_sendUserID; }
	const std::string& GetRecvUserID() { return m_recvUserID; }
	const std::string& GetChatMessage() { return m_chatMessage; }
};

