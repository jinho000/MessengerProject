#pragma once
#include "Singleton.h"

class PacketHandler : public Singleton<PacketHandler>
{
	friend class Singleton;

private: // member var
	

private: // default
	PacketHandler();
	~PacketHandler();

public: // member Func
	void Dispatch(const std::vector<uint8_t> _buffer);
};

