#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Window.h"
#include "Scene.h"

class ImGuiLayer
{
public:
	ImGuiLayer(Window* window = nullptr, Scene* scene = nullptr) : m_Window(window), m_Scene(scene) 
	{
		if(window != nullptr && scene != nullptr)
			Init();
	}
	~ImGuiLayer() {}

	void Init();
	
	void StartFrame();
	void Update();

	void Render();
private:
	Window* m_Window = nullptr;
	Scene* m_Scene = nullptr;

	float m_MainScale = 0;
	bool m_ShowAddObject = true;

	int m_ParentObject = 0;
	float* m_PlacementPosition = new float[3] {0, 0, 0};
	float* m_RotationPlacement = new float[3] {0, 0, 0};
	float* m_ScalePlacement = new float[3] {1, 1, 1};

	int m_CurrentInspectorGameObject = 0;

	void BuildHiearchyText(SceneGraphNode* node);
	float m_Indentation = 10.0f;
};