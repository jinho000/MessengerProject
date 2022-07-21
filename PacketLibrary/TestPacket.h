#pragma once
#include "RecvPacket.h"

class TestPacket : public RecvPacket
{
private:
	int m_test;

public: // default
	TestPacket();
	~TestPacket() = default;

public:
	void Deserialize() override;
};

