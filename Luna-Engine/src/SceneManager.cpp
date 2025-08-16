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

	LoadRelations(jsonData, jsonData["relations"][0], 0);
}

void SceneManager::LoadRelations(const nlohmann::json& originalData, const nlohmann::json& jsonData, unsigned int parentObjectID)
{
	unsigned int objectID = m_Scene->AddObject(parentObjectID);

	m_Scene->AddComponent<Transform>(objectID);
	for (nlohmann::json componentData : originalData["objects"][jsonData["ObjectID"].get<unsigned int>()])
	{
		if (componentData["component-type"] == "NameComponent")
		{
			m_Scene->AddComponent<NameComponent>(objectID, componentData["component-args"][0]);
		}
	}

	for (nlohmann::json data : jsonData["Children"])
		LoadRelations(originalData, data, objectID);
}

void SceneManager::UnloadCurrentScene()
{
	if (m_Scene == nullptr)
		return;
	m_Scene->DestroyScene();
	delete m_Scene;
}