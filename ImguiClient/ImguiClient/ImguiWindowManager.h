#pragma once
#include <vector>

enum class WINDOW_UI
{
	LOGIN,
	MAIN,
	JOIN,
	END
};

class ImguiWindow;
class ImguiWindowManager
{
private: // member var
	static ImguiWindowManager* pInst;

	ImguiWindow*	m_imguiWindow[static_cast<int>(WINDOW_UI::END)];
	WINDOW_UI		m_currentUI;

private: // default
	ImguiWindowManager();
	~ImguiWindowManager();

	ImguiWindowManager(const ImguiWindowManager& _other) = delete;
	ImguiWindowManager(ImguiWindowManager&& _other) = delete;
	ImguiWindowManager& operator=(const ImguiWindowManager& _other) = delete;
	ImguiWindowManager& operator=(const ImguiWindowManager&& _other) = delete;

public: // member Func
	static void CreateIntance();
	static void Destroy();
	static ImguiWindowManager* GetInst() { return pInst; }

public:
	void UpdateWindow();
	void ChangeMainWindow(WINDOW_UI _window);

	ImguiWindow* GetImguiWindow(WINDOW_UI _windowUIType) { return m_imguiWindow[static_cast<int>(_windowUIType)]; }
};

