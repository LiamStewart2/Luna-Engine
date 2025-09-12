#pragma once

#include "../imgui/imgui.h"

class ImGuiPanel
{
public:
	ImGuiPanel();
	~ImGuiPanel();

	void StartFrame();
	void Update();
	void EndFrame();

	

protected:
	bool m_Show;
};

