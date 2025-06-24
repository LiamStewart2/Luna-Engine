#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::Init()
{
	epicObject.AddComponent<CoolComponent>();
}

void Scene::Update()
{
	epicObject.Update();
}

void Scene::PushBackObject(Mesh* mesh)
{
	objectBuffer.push_back(mesh);
}

void Scene::LoadAssets()
{

}
