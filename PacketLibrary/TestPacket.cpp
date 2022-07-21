#include "pch.h"
#include "TestPacket.h"

TestPacket::TestPacket()
	: RecvPacket(PACKET_TYPE::TEST_PACKET)
	, m_test(1)
{
}

void TestPacket::Deserialize()
{
	// IOBuffer의 데이터를 복사
	UINT currPos = 0;
	
}
