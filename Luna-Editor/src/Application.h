#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "ECS/ECS.h"
#include "Renderer/Renderer.h"
#include "Core/AssetLoader.h"
#include "ImGuiLayer.h"

#include "Core/Globals.h"

#include "Core/LunaWindow.h"
#include "Core/SceneManager.h"
#include "Core/Scene.h"
#include "Renderer/Material.h"
#include "Renderer/Light.h"
#include "Renderer/Shader.h"
#include "Core/Camera.h"

class Application
{
public:
	Application();
	~Application();
private:
	LunaWindow* window;

	int Init();
	void Terminate();

	void MainLoop();

	void HandleInput();
	void Update();
	void Render();
	
	Renderer renderer;
	SceneManager sceneManager;
	ImGuiLayer imGuiLayer;

	float main_scale;
	bool showAddObjectWindow = true;
	float* placementPosition = new float[3] {1, 1, 1};
};