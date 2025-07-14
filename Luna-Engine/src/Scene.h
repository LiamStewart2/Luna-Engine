#pragma once

#include <vector>

#include "Window.h"
#include "ECS/ECS.h"
#include "LinkedList.h"
#include "AssetLoader.h"
#include "Mesh.h"
#include "AssetLoader.h"
#include "Renderer.h"
#include "LightManager.h"

class Scene
{
public:
	Scene();
	~Scene();

	void Init(Window* window);
	void LoadAssets();

	void Update();
	void Render(Renderer* renderer);

	void DestroyScene();

private:
	EntityComponentSystem ECS;

	Window* window;

	Shader shader;
	Shader depthmapShader;

	Camera camera = Camera(glm::vec3(0, 2, -3), glm::vec3(0, 0, 0));
	LightManager lightManager;

	Light light = Light(glm::vec3(0, 10, 10), glm::vec3(0, 1, 1), glm::vec3(1));
	Material material;

	Mesh planeMesh;
	Mesh monkeyMesh;

	Texture defaultTexture;
	Texture stoneTexture;

};

