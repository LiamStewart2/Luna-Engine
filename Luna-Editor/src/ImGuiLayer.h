#pragma once

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_dx11.h"
#include "imgui/misc/cpp/imgui_stdlib.h"
#include "imgui/ImGuizmo.h"
#include "GLM/gtx/matrix_decompose.hpp"

#include "Luna.h"
#include "EditorCamera.h"
#include "ProjectManager.h"
#include "Panels/ImGuiPanel.h"

#include "Panels/InspectorPanel.h"
#include "Panels/MaterialEditorPanel.h"
#include "Panels/HierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Panels/ScenePanel.h"
#include "Panels/GamePanel.h"
#include "Panels/ShaderGraphEditorPanel.h"
#include "Panels/NewProjectPanel.h"

#include <math.h>
#include <unordered_map>

class ImGuiLayer
{
public:
	ImGuiLayer(LunaWindow* window = nullptr, Luna::AssetManager* assetManager = nullptr, SceneManager* sceneManager = nullptr, ProjectManager* projectManager = nullptr) 
		: m_Window(window), m_AssetManager(assetManager), m_SceneManager(sceneManager), m_ProjectManager(projectManager)
	{
		if(window != nullptr && m_AssetManager != nullptr && m_SceneManager != nullptr && m_ProjectManager != nullptr)
		{
			Init();

			m_InspectorPanel = InspectorPanel(m_SceneManager);
			m_HierarchyPanel = HierarchyPanel(m_SceneManager);
			m_MaterialEditorPanel = MaterialEditorPanel(m_SceneManager);
			m_ContentBrowserPanel = ContentBrowserPanel(m_SceneManager, m_ProjectManager->GetOpenProject().m_WorkingDirectory);
			m_ScenePanel = ScenePanel(m_SceneManager);
			m_GamePanel = GamePanel(m_SceneManager);
			m_ShaderGraphPanel = ShaderGraphEditorPanel(m_SceneManager); m_ShaderGraphPanel.Close();
			m_NewProjectPanel = NewProjectPanel(m_SceneManager, m_ProjectManager); m_NewProjectPanel.Close();
		}
	}
	~ImGuiLayer() {}

	void Init();
	
	void StartFrame();
	void Update(ObjectTransformPairing<Camera>& camera, std::shared_ptr<Luna::IFramebuffer> framebuffer, bool& runtime, std::shared_ptr<Luna::IFramebuffer> gameFramebuffer);

	void Render();
private:
	LunaWindow* m_Window = nullptr;
	SceneManager* m_SceneManager = nullptr;
	ProjectManager* m_ProjectManager = nullptr;
	Luna::AssetManager* m_AssetManager = nullptr;


	ImFont* font = nullptr;

	float m_MainScale = 0;
	bool m_ShowGame = true;
	bool m_ShowContentBrowser = true;

	bool m_SaveShortcutUsed = false;

	unsigned int m_CurrentInspectorGameObject = 0;

	ImGuizmo::OPERATION m_CurrentOperation = ImGuizmo::TRANSLATE;

	std::vector<std::pair<ACTIONS, std::string>> m_Actions;

	InspectorPanel m_InspectorPanel;
	HierarchyPanel m_HierarchyPanel;
	MaterialEditorPanel m_MaterialEditorPanel;
	ContentBrowserPanel m_ContentBrowserPanel;
	ScenePanel m_ScenePanel; 
	GamePanel m_GamePanel;
	ShaderGraphEditorPanel m_ShaderGraphPanel;
	NewProjectPanel m_NewProjectPanel;
};