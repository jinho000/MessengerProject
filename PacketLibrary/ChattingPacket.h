#pragma once
#include "PacketBase.h"


class ChattingPacket : public PacketBase
{
private: // member var
	ChatMessage m_chatMessage;

public: // default
	ChattingPacket();
	ChattingPacket(const ChatMessage& _chatMessage);
	~ChattingPacket();

protected:
	virtual size_t GetContentPacketSize() override;

public:
	Serializer Serialize() override;
	void Deserialize(const Serializer& _serializer) override;

public:
	ChatMessage& GetChattingMessage() { return m_chatMessage; }
};

