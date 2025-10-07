#pragma once

#include "ECS/ECS.h"
#include "Renderer/Renderer.h"
#include "Core/AssetLoader.h"
#include "ImGuiLayer.h"

#include "Luna.h"

#include <windows.h>
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

	int Init();
	void Terminate();

	void MainLoop();

	void HandleInput();
	void Update();
	void Render();
	
	Renderer renderer;
	
	std::shared_ptr<Luna::RendererContext> m_RendererContext;
	SceneManager sceneManager;
	ImGuiLayer imGuiLayer;

	std::shared_ptr<Luna::IFramebuffer> m_Framebuffer = nullptr;
	std::shared_ptr<Luna::IMesh> m_Mesh = nullptr;

	EditorCamera editorCamera = EditorCamera(glm::vec3(0, 3, 6.5), glm::vec3(-90, -20, 0));
	// DX11 TEMP STUFF
};