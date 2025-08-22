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

void SceneManager::SaveCurrentScene()
{
	if(m_Scene == nullptr)
		return;

	nlohmann::json jsonData;
	
	jsonData["scene-name"] = m_Scene->GetSceneName();

	// Save objects to json data

	jsonData["objects"] = nlohmann::json::array();

	for (int i = 0; i < m_Scene->GetGameObjects()->size(); i++)
	{
		nlohmann::json objectComponents = nlohmann::json::array();

		if (m_Scene->GetECS()->HasComponent<NameComponent>(m_Scene->GetGameObjects()->at(i)))
		{
			NameComponent* component = m_Scene->GetECS()->GetObjectComponent<NameComponent>(m_Scene->GetGameObjects()->at(i));
			objectComponents.push_back({
				{"component-type", "NameComponent"},
				{"component-args", {component->m_Name}}
			});
		}

		if (m_Scene->GetECS()->HasComponent<Transform>(m_Scene->GetGameObjects()->at(i)))
		{
			Transform* component = m_Scene->GetECS()->GetObjectComponent<Transform>(m_Scene->GetGameObjects()->at(i));
			objectComponents.push_back({
				{"component-type", "TransformComponent"},
				{"component-args", {
					component->position.x, component->position.y, component->position.z,
					component->rotation.x, component->rotation.y, component->rotation.z,
					component->scale.x, component->scale.y, component->scale.z
				}}
			});
		}

		jsonData["objects"].push_back(objectComponents);
	}


	SceneGraphNode* sceneGraph = m_Scene->GetSceneGraph();
	for(int i = 0; i < sceneGraph->getNodes()->size(); i++)
		SaveSceneNode(jsonData, sceneGraph->getNodes()->at(i));
}

void SceneManager::SaveSceneNode(nlohmann::json& data, SceneGraphNode* node)
{

}


void SceneManager::LoadNewScene(const char* filepath)
{
	// Unload the current scene if it exists
	UnloadCurrentScene();

	double startTime = glfwGetTime();

	// Load the scene file using json :: ToDo change json to a more suitable format
	std::ifstream file(filepath);
	nlohmann::json jsonData = nlohmann::json::parse(file);

	std::cout << "Loading Scene -- Scene name: " << jsonData["scene-name"] << std::endl;

	m_Scene = new Scene();
	m_Scene->Init(&assetManager, jsonData["scene-name"]);

	LoadRelations(jsonData, jsonData["relations"][0], 0);

	std::cout << "time to load " << jsonData["scene-name"] << " - " << glfwGetTime() - startTime << std::endl;
}

void SceneManager::LoadRelations(const nlohmann::json& originalData, const nlohmann::json& jsonData, unsigned int parentObjectID)
{

	unsigned int objectID = m_Scene->AddObject(parentObjectID);

	//LoadComponents
	for (nlohmann::json componentData : originalData["objects"][jsonData["ObjectID"].get<unsigned int>()])
	{
		if (componentData["component-type"] == "NameComponent")
			m_Scene->AddComponent<NameComponent>(objectID, componentData["component-args"][0]);
		else if(componentData["component-type"] == "TransformComponent")
		{
			glm::vec3 position = glm::vec3(componentData["component-args"][0], componentData["component-args"][1], componentData["component-args"][2]);
			glm::vec3 rotation = glm::vec3(componentData["component-args"][3], componentData["component-args"][4], componentData["component-args"][5]);
			glm::vec3 scale = glm::vec3(componentData["component-args"][6], componentData["component-args"][7], componentData["component-args"][8]);

			m_Scene->AddComponent<Transform>(objectID, position, rotation, scale);
		}
		else if(componentData["component-type"] == "MeshComponent")
		{
			std::shared_ptr<Mesh> mesh = assetManager.GetMesh(std::string("Assets/Models/") + componentData["component-args"][0].get<std::string>());
			std::shared_ptr<Shader> shader = assetManager.GetShader(std::string("Assets/Shaders/") + componentData["component-args"][1].get<std::string>());
			std::shared_ptr<Texture> texture = assetManager.GetTexture(std::string("Assets/Textures/") + componentData["component-args"][2].get<std::string>());
			m_Scene->AddComponent<MeshComponent>(objectID, mesh.get(), shader.get(), &defaultMat, texture.get());
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