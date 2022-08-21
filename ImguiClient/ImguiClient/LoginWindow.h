#pragma once
#include "ImguiWindow.h"
#include "JoinWindow.h"
#include <string>

class LoginWindow : public ImguiWindow
{
private: // member var
	enum
	{
		BUFFER_SIZE = 64
	};

	char		m_IDBuffer[BUFFER_SIZE];
	char		m_PWBuffer[BUFFER_SIZE];
	JoinWindow	m_joinModal;

public: // default
	LoginWindow();
	~LoginWindow() = default;

public:
	static void DispatchLoginResultPacket(std::unique_ptr<PacketBase> _packet);

public: // member Func
	virtual void UpdateWindow() override;

	JoinWindow* GetJoinWindow() { return &m_joinModal; }
};

