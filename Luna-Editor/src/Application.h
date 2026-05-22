#pragma once

#include "ECS/ECS.h"
#include "Core/AssetLoader.h"
#include "ImGuiLayer.h"
#include "ProjectManager.h"

#include "Luna.h"

#include <windows.h>
#include <thread>
#include <d3d11_4.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

class Application
{
public:
	Application();
	~Application();
private:
	LunaWindow* window;
	bool runtime = false;
	bool m_Running = false;

	int Init();
	void Terminate();

	void MainLoop();

	void HandleInput();
	void Update();
	void Render();
	
	void ResizeCallback(GLFWwindow* handle, int width, int height);
	
	std::shared_ptr<Luna::RendererContext> m_RendererContext;
	SceneManager sceneManager;
	ImGuiLayer imGuiLayer;
	ProjectManager m_ProjectManager;

	std::shared_ptr<Luna::IFramebuffer> m_Framebuffer = nullptr;
	std::shared_ptr<Luna::IFramebuffer> m_GameFramebuffer = nullptr;
	std::shared_ptr<Luna::IFramebuffer> m_Backbuffer = nullptr;

	EditorCamera editorCamera = EditorCamera(glm::vec3(0, 3, 6.5), glm::vec3(-90, -20, 0));
	// DX11 TEMP STUFF
};