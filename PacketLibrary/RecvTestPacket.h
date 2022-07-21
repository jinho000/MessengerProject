#pragma once
#include "PacketBase.h"

class RecvTestPacket : public PacketBase
{
private:
	int m_test;

public: // default
	RecvTestPacket(std::vector<uint8_t>& _IOBuffer);
	~RecvTestPacket() = default;

private:
	void Deserialize() override;
	void Serialize() override;

public:
	int GetTest() { return m_test; }

};

