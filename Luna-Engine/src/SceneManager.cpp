#include "SceneManager.h"


SceneManager::SceneManager()
{

}

SceneManager::~SceneManager()
{

}

void SceneManager::Update()
{
	m_Scene->Update();
}

void SceneManager::Render(Renderer* renderer)
{
	m_Scene->Render(renderer);
}

void SceneManager::LoadNewScene(const char* filepath)
{

}

void SceneManager::UnloadCurrentScene()
{
	m_Scene->DestroyScene();
	delete m_Scene;
}