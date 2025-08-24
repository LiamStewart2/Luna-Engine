#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Window.h"
#include "SceneManager.h"

#include <unordered_map>

class ImGuiLayer
{
public:
	ImGuiLayer(Window* window = nullptr, SceneManager* sceneManager = nullptr) : m_Window(window), m_SceneManager(sceneManager)
	{
		if(window != nullptr && m_SceneManager != nullptr)
			Init();
	}
	~ImGuiLayer() {}

	void Init();
	
	void StartFrame();
	void Update();

	void Render();
private:
	Window* m_Window = nullptr;
	SceneManager* m_SceneManager = nullptr;

	float m_MainScale = 0;
	bool m_ShowAddObject = true;

	unsigned int NameCharacterLimit = 100;
	float Indentation = 10.0f;

	int m_ParentObject = 0;
	char* m_Name = new char[NameCharacterLimit]();
	float* m_PlacementPosition = new float[3] {0, 0, 0};
	float* m_RotationPlacement = new float[3] {0, 0, 0};
	float* m_ScalePlacement = new float[3] {1, 1, 1};

	char* filepathForScene = new char[50]("");

	int m_CurrentInspectorGameObject = 0;

	int m_ParentObjectID = 0;
	char* m_ObjectName = new char[50]("");

	void BuildHiearchyText(SceneGraphNode* node, std::unordered_map<unsigned int, NameComponent*>* names);
};