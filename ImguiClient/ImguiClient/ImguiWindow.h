#pragma once

class ImguiWindow
{
public: // default
	ImguiWindow();
	virtual ~ImguiWindow() = 0;

	ImguiWindow(const ImguiWindow& _other) = delete;
	ImguiWindow(ImguiWindow&& _other) = delete;
	ImguiWindow& operator=(const ImguiWindow& _other) = delete;
	ImguiWindow& operator=(const ImguiWindow&& _other) = delete;

public: // member Func
	virtual void UpdateWindow() = 0;
};

