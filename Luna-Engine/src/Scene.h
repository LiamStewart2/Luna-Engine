#pragma once

#include <vector>

#include "TransformationManager.h"
#include "Window.h"
#include "SceneGraph.h"
#include "ECS/ECS.h"
#include "AssetManager.h"
#include "LinkedList.h"
#include "Mesh.h"
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

	void AddObject(unsigned int parent = 0, std::string name = "Game Object", glm::vec3 position = glm::vec3(0), glm::vec3 rotation = glm::vec3(0), glm::vec3 scale = glm::vec3(1));

	EntityComponentSystem* GetECS() {return &ECS;}
	SceneGraph* GetSceneGraph() {return &sceneGraph; }

	void DestroyScene();

private:
	EntityComponentSystem ECS;
	SceneGraph sceneGraph;
	std::vector<unsigned int> gameObjects;

	TransformationManager transformationManager;
	AssetManager assetManager;

	Window* window;

	Camera* camera = new PerspectiveCamera();
	LightManager lightManager;

	Light light = Light(glm::vec3(0, 10, 10), glm::vec3(0, 1, 1), glm::vec3(1));
	Material material;
};

