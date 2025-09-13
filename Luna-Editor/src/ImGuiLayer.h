#pragma once

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/misc/cpp/imgui_stdlib.h"
#include "imgui/ImGuizmo.h"
#include "GLM/gtx/matrix_decompose.hpp"

#include "Luna.h"
#include "EditorCamera.h"
#include "Panels/ImGuiPanel.h"

#include "Panels/InspectorPanel.h"
#include "Panels/HierarchyPanel.h"
#include "Panels/ScenePanel.h"

#include <math.h>
#include <unordered_map>

enum ACTIONS
{
	LOADSCENE
};

class ImGuiLayer
{
public:
	ImGuiLayer(LunaWindow* window = nullptr, AssetManager* assetManager = nullptr, SceneManager* sceneManager = nullptr) : m_Window(window), m_AssetManager(assetManager), m_SceneManager(sceneManager)
	{
		if(window != nullptr && m_AssetManager != nullptr && m_SceneManager != nullptr)
		{
			Init();

			m_InspectorPanel = InspectorPanel(m_SceneManager);
			m_HierarchyPanel = HierarchyPanel(m_SceneManager);
			m_ScenePanel = ScenePanel(m_SceneManager);
		}
	}
	~ImGuiLayer() {}

	void Init();
	
	void StartFrame();
	void Update(ObjectTransformPairing<Camera>& camera, FrameBuffer* sceneFramebuffer, FrameBuffer* gameFramebuffer);

	void Render();
private:
	LunaWindow* m_Window = nullptr;
	SceneManager* m_SceneManager = nullptr;
	AssetManager* m_AssetManager = nullptr;

	float m_MainScale = 0;
	bool m_ShowGame = true;
	bool m_ShowContentBrowser = true;

	bool m_SaveShortcutUsed = false;

	unsigned int m_CurrentInspectorGameObject = 0;

	ImGuizmo::OPERATION m_CurrentOperation = ImGuizmo::TRANSLATE;

	std::vector<std::pair<ACTIONS, std::string>> m_Actions;

	InspectorPanel m_InspectorPanel;
	HierarchyPanel m_HierarchyPanel;
	ScenePanel m_ScenePanel;
};