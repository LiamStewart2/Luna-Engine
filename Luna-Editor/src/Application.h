#pragma once

#include "ECS/ECS.h"
#include "Renderer/Renderer.h"
#include "Core/AssetLoader.h"
#include "ImGuiLayer.h"

#include "Luna.h"

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

	EditorCamera editorCamera = EditorCamera(glm::vec3(0, 3, 6.5), glm::vec3(-90, -20, 0));
};