#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"

#include "AssetLoader.h"

class Application
{
public:
	Application();
	~Application();
private:
	GLFWwindow* window;

	int Init();
	void Terminate();

	void MainLoop();

	void HandleInput();
	void Update();
	void Render();

	Mesh monkeyMesh;
};