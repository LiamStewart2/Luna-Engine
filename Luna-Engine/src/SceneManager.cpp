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
	// Unload the current scene if it exists
	UnloadCurrentScene();

	// Load the scene file using json :: ToDo change json to a more suitable format
	std::ifstream file(filepath);
	nlohmann::json jsonData = nlohmann::json::parse(file);

	std::cout << "Scene name: " << jsonData["scene-name"] << std::endl;

}

void SceneManager::UnloadCurrentScene()
{
	if (m_Scene == nullptr)
		return;
	m_Scene->DestroyScene();
	delete m_Scene;
}