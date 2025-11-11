#pragma once

#include <vector>

#include "TransformationManager.h"
#include "../Scripting/ScriptManager.h"
#include "LunaWindow.h"
#include "SceneGraph.h"
#include "../ECS/ECS.h"
#include "AssetManager.h"
#include "LinkedList.h"

class Scene
{
public:
	Scene();
	~Scene();

	void Init(Luna::AssetManager* _assetManager, std::string _sceneName);
	void LoadAssets();

	void Update(bool runtime = false);

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

	ScriptManager* GetScriptManager() {return &scriptManager;}

	std::string GetSceneName() {return sceneName;}
	std::string filepath = "";

	CameraComponent* GetMainCameraComponent();

	void DestroyScene();
private:
	EntityComponentSystem ECS;
	SceneGraph sceneGraph;
	std::vector<unsigned int> gameObjects;

	std::string sceneName = "";

	ScriptManager scriptManager;
	TransformationManager transformationManager;

	Luna::AssetManager* assetManager;
};

