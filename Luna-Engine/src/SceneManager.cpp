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

	std::cout << "Loading Scene -- Scene name: " << jsonData["scene-name"] << std::endl;

	m_Scene = new Scene();
	m_Scene->Init(Window::m_FocusedWindow);


}

void SceneManager::LoadRelations(const nlohmann::json& jsonData)
{
	for (auto data : jsonData)
	{
		
	}
}

void SceneManager::UnloadCurrentScene()
{
	if (m_Scene == nullptr)
		return;
	m_Scene->DestroyScene();
	delete m_Scene;
}