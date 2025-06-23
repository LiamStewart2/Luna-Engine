#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "AssetLoader.h"

#include "Globals.h"

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
	GLFWwindow* window;

	int Init();
	void LoadAssets();
	void Terminate();

	void MainLoop();

	void HandleInput();
	void Update();
	void Render();
	
	Renderer renderer;
	Shader shader;
	Camera camera = Camera(glm::vec3(0, 0, -3), glm::vec3(0, 0, 0));
	
	Light light;
	Material material;

	Scene scene;

	Mesh monkeyMesh;

	Texture shrekTexture;
};