#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::Init()
{
	LoadAssets();

	epicObject = GameObject(glm::vec3({0, 0, 0}));
	epicObject.AddComponent<CoolComponent>();
}

void Scene::LoadAssets()
{

}

void Scene::Update()
{
	epicObject.Update();
}

void Scene::Render(Renderer* renderer)
{
	epicObject.OnRender(renderer);
}

