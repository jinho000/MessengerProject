#pragma once
#include "PacketBase.h"

class ReadChattingPacket : public PacketBase
{
private: // member var
	std::string m_sendUserID;
	std::string m_recvUserID;
	int			m_readCount;

public: // default
	ReadChattingPacket();
	ReadChattingPacket(const std::string& _sendUserID, const std::string& _recvUserID, int _readCount);
	~ReadChattingPacket();

protected:
	virtual size_t GetContentPacketSize() override;

public:
	Serializer Serialize() override;
	void Deserialize(const Serializer& _serializer) override;

	const std::string& GetSendUserID() { return m_sendUserID; }
	const std::string& GetRecvUserID() { return m_recvUserID; }
	int GetReadCount() { return m_readCount; }
};

