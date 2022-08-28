#pragma once
#include <string>
#include "ImguiWindow.h"
#include "NetworkManager.h"
#include <PacketLibrary/enum.h>

class JoinWindow : public ImguiWindow
{
private: // member var
	enum
	{
		BUFFER_SIZE = 64
	};

	char		m_IDBuffer[BUFFER_SIZE];
	char		m_PWBuffer[BUFFER_SIZE];
	bool		m_active;
	bool		m_bCheckID;

	std::string m_checkResult;
	std::string m_JoinResult;

public: // default
	JoinWindow();
	~JoinWindow();

public:
	static void DispatchJoinResultPacket(std::unique_ptr<PacketBase> _packet);
	static void DispatchIDCheckResultPacket(std::unique_ptr<PacketBase> _packet);

public: // member Func
	void UpdateWindow();

	void Active();
};

