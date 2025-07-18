#pragma once

#include "Window.h"
#include "Scene.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

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
	float* m_PlacementPosition = new float[3] {1, 1, 1};
};