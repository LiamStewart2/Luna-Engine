#pragma once

#include "Luna.h"

#include "../imgui/imgui.h"
#include "../imgui/backends/imgui_impl_glfw.h"
#include "../imgui/backends/imgui_impl_dx11.h"
#include "../imgui/misc/cpp/imgui_stdlib.h"

enum ACTIONS
{
	LOADSCENE
};

class ImGuiPanel
{
public:
	ImGuiPanel(SceneManager* sceneManager = nullptr) : m_SceneManager(sceneManager) {}
	~ImGuiPanel() {}

	void Open() {m_Show = true;}
	void Close() {m_Show = false;}

	virtual void StartFrame() {}
	virtual void Update(unsigned int& inspectorID) {}
	virtual void EndFrame(unsigned int& inspectorID) {}
protected:
	SceneManager* m_SceneManager;
	bool m_Show = true;
};

