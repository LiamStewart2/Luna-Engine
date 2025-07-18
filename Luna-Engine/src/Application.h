#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "ECS/ECS.h"
#include "Renderer.h"
#include "AssetLoader.h"

#include "Globals.h"

#include "Window.h"
#include "Scene.h"
#include "Material.h"
#include "Light.h"
#include "Time.h"
#include "Shader.h"
#include "Camera.h"

class Application
{
public:
	Application();
	~Application();
private:
	Window* window;

	int Init();
	void Terminate();

	void MainLoop();

	void HandleInput();
	void Update();
	void Render();
	
	Renderer renderer;
	Scene scene;

	float main_scale;
	bool showAddObjectWindow = true;
	float* placementPosition = new float[3] {1, 1, 1};
};