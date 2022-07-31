#pragma once

// 용도 :
// 분류 :
// 첨언 :
class ChattingPacket
{
private: // member var
	

public: // default
	ChattingPacket();
	~ChattingPacket();

	ChattingPacket(const ChattingPacket& _other) = delete;
	ChattingPacket(ChattingPacket&& _other) = delete;

protected:
	ChattingPacket& operator=(const ChattingPacket& _other) = delete;
	ChattingPacket& operator=(const ChattingPacket&& _other) = delete;

private:

public: // member Func
};

