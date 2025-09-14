#pragma once

#include <vector>

#include "TransformationManager.h"
#include "LunaWindow.h"
#include "SceneGraph.h"
#include "../ECS/ECS.h"
#include "AssetManager.h"
#include "LinkedList.h"
#include "../Renderer/Mesh.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/LightManager.h"

class Scene
{
public:
	Scene();
	~Scene();

	void Init(AssetManager* _assetManager, LightManager* _lightManager, std::string _sceneName);
	void LoadAssets();

	void Update();
	void Render(Renderer* renderer, FrameBuffer* framebuffer, ObjectTransformPairing<Camera>& camera);

	unsigned int AddObject(unsigned int parent = 0);
	template <typename T, typename... Args>
	T* AddComponent(unsigned int gameObject, Args&&... args)
	{
		return ECS.AddComponent<T>(gameObject, std::forward<Args>(args)...);
	}

	void DestroyGameObject(unsigned int gameObject = 0);

	EntityComponentSystem* GetECS() {return &ECS;}
	SceneGraph* GetSceneGraph() {return &sceneGraph; }
	std::vector<unsigned int>* GetGameObjects() {return &gameObjects;}

	std::string GetSceneName() {return sceneName;}
	std::string filepath = "";

	void DestroyScene();
private:
	EntityComponentSystem ECS;
	SceneGraph sceneGraph;
	std::vector<unsigned int> gameObjects;

	std::string sceneName = "";

	TransformationManager transformationManager;

	AssetManager* assetManager;
	LightManager* lightManager;

	Material material = Material(glm::vec3(1));

};

