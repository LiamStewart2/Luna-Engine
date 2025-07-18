#pragma once

#include <vector>

#include "TransformationManager.h"
#include "Window.h"
#include "SceneGraph.h"
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

	void AddObject(glm::vec3 position = glm::vec3(0), glm::vec3 rotation = glm::vec3(0), glm::vec3 scale = glm::vec3(1));

	void DestroyScene();

private:
	EntityComponentSystem ECS;
	SceneGraph sceneGraph;
	std::vector<unsigned int> gameObjects;

	TransformationManager transformationManager;

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

