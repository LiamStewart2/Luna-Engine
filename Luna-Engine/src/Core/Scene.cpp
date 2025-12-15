#include "Scene.h"
#include "SceneManager.h"

Scene::Scene()
{

}

Scene::~Scene()
{
}

void Scene::Init(Luna::AssetManager* _assetManager, std::string _sceneName)
{
	assetManager = _assetManager; sceneName = _sceneName;
	LoadAssets();

	unsigned int sceneObject = 0;
	ECS.AddComponent<NameComponent>(sceneObject, "Scene");
	ECS.AddComponent<Transform>(sceneObject, glm::vec3(0, 0, 0));
	sceneGraph = SceneGraph(sceneObject);
	gameObjects = std::vector<unsigned int>({0});
}

void Scene::LoadAssets()
{

}

void Scene::Update(bool runtime)
{
	if(runtime)
	{
		physicsSystem.Update(&ECS);
		scriptManager.OnSceneUpdate(&ECS);
	}
	transformationManager.UpdateTransformationMatricies(&sceneGraph, &ECS);
}

unsigned int Scene::AddObject(unsigned int parent)
{
	unsigned int object = gameObjects[gameObjects.size() - 1] + 1;

	sceneGraph.InsertNode(object, sceneGraph.GetNode(parent, nullptr));
	gameObjects.push_back(object);

	return object;
}

void Scene::DestroyGameObject(unsigned int gameObject)
{
	sceneGraph.RemoveNode(gameObject, &ECS, &gameObjects);
}


void Scene::DestroyScene()
{
	sceneGraph.RemoveNode(0, &ECS, &gameObjects);
}

CameraComponent* Scene::GetMainCameraComponent()
{
	std::unordered_map<unsigned int, CameraComponent>& cameras = ECS.GetAllComponentsOfType<CameraComponent>();
	for (auto& [id, cameraComponent] : cameras)
	{
		if (cameraComponent.m_MainCamera)
			return &cameraComponent;
	}
	return nullptr;
}