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

	void Init(Window* window, AssetManager* _assetManager);
	void LoadAssets();

	void Update();
	void Render(Renderer* renderer);

	unsigned int AddObject(unsigned int parent = 0);
	template <typename T, typename... Args>
	T* AddComponent(unsigned int gameObject, Args&&... args)
	{
		return ECS.AddComponent<T>(gameObject, std::forward<Args>(args)...);
	}

	EntityComponentSystem* GetECS() {return &ECS;}
	SceneGraph* GetSceneGraph() {return &sceneGraph; }

	void DestroyScene();

private:
	EntityComponentSystem ECS;
	SceneGraph sceneGraph;
	std::vector<unsigned int> gameObjects;

	TransformationManager transformationManager;
	AssetManager* assetManager;

	Window* window;

	Camera* camera = new PerspectiveCamera();
	LightManager lightManager;

	Material material;
};

