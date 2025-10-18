#include "SceneManager.h"


SceneManager::SceneManager()
{

}

SceneManager::~SceneManager()
{
}

void SceneManager::Update(bool runtime)
{
	m_Scene->Update(runtime);
}

void SceneManager::SaveScene()
{
	if (m_Scene == nullptr)
		return;
	std::cout << "saved scene" << std::endl;
	SaveCurrentSceneAs(m_Scene->filepath);
}

void SceneManager::SaveCurrentSceneAs(std::string optionalPath)
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
					component->rotation.w, component->rotation.x, component->rotation.y, component->rotation.z,
					component->scale.x, component->scale.y, component->scale.z
				}}
			});
		}

		if (m_Scene->GetECS()->HasComponent<MeshComponent>(m_Scene->GetGameObjects()->at(i)))
		{
			MeshComponent* component = m_Scene->GetECS()->GetObjectComponent<MeshComponent>(m_Scene->GetGameObjects()->at(i));
			objectComponents.push_back({
				{"component-type", "MeshComponent"},
				{"component-args", {component->mesh->m_Path, component->shader->m_Path, component->texture->GetTexturePacket()->path}}});
		}

		if (m_Scene->GetECS()->HasComponent<CameraComponent>(m_Scene->GetGameObjects()->at(i)))
		{
			CameraComponent* component = m_Scene->GetECS()->GetObjectComponent<CameraComponent>(m_Scene->GetGameObjects()->at(i));
			objectComponents.push_back({
				{"component-type", "CameraComponent"},
				{"component-args", {
					component->m_Camera->m_EditorBackgroundColour.x, component->m_Camera->m_EditorBackgroundColour.y,
					component->m_Camera->m_EditorBackgroundColour.z, component->m_Camera->m_EditorBackgroundColour.w,
					component->m_MainCamera}}
			});
		}

		if (m_Scene->GetECS()->HasComponent<LightComponent>(m_Scene->GetGameObjects()->at(i)))
		{
			LightComponent* component = m_Scene->GetECS()->GetObjectComponent<LightComponent>(m_Scene->GetGameObjects()->at(i));
			objectComponents.push_back({
				{"component-type", "LightComponent"},
				{"component-args", {component->m_Light.m_LightColour.x, component->m_Light.m_LightColour.y, component->m_Light.m_LightColour.z}}
			});
		}

		if (m_Scene->GetECS()->HasComponent<ScriptComponent>(m_Scene->GetGameObjects()->at(i)))
		{
			ScriptComponent* component = m_Scene->GetECS()->GetObjectComponent<ScriptComponent>(m_Scene->GetGameObjects()->at(i));
			objectComponents.push_back({
				{"component-type", "ScriptComponent"},
				{"component-args", {component->m_Script->GetFilepath()}}
			});
		}

		jsonData["objects"].push_back(objectComponents);
	}
	
	// Load relations from scene graph
	jsonData["relations"] = nlohmann::json::array();
	SceneGraphNode* sceneGraph = m_Scene->GetSceneGraph();

	SaveSceneNode(jsonData["relations"], sceneGraph);

	// Write to file
	if(optionalPath == "")
	{
		std::ofstream file(std::string(m_Scene->GetSceneName()));
		file << jsonData.dump(4);
	}
	else
	{
		std::ofstream file(optionalPath);
		file << jsonData.dump(4);
	}
}

void SceneManager::SaveSceneNode(nlohmann::json& data, SceneGraphNode* node)
{
	for (int i = 0; i < node->getNodes()->size(); i++)
	{
		data.push_back(
			{
				{"ObjectID", node->getNodes()->at(i)->GetGameObject()},
				{"Children", nlohmann::json::array()}
			});
		SaveSceneNode(data[data.size() - 1]["Children"], node->getNodes()->at(i));
	}
}


void SceneManager::LoadNewScene(const char* filepath)
{

	// Unload the current scene if it exists
	UnloadCurrentScene();

	double startTime = glfwGetTime();

	std::cout << filepath << std::endl;

	// Load the scene file using json :: ToDo change json to a more suitable format
	std::ifstream file(filepath);
	nlohmann::json jsonData = nlohmann::json::parse(file);

	std::cout << "Loading Scene -- Scene name: " << jsonData["scene-name"] << std::endl;

	m_Scene = new Scene();
	m_Scene->filepath = std::string(filepath);
	m_Scene->Init(&assetManager, jsonData["scene-name"]);

	for(nlohmann::json data : jsonData["relations"])
		LoadRelations(jsonData, data, 0);

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
			glm::quat rotation = glm::quat(componentData["component-args"][3], componentData["component-args"][4], componentData["component-args"][5], componentData["component-args"][6]);
			glm::vec3 scale = glm::vec3(componentData["component-args"][7], componentData["component-args"][8], componentData["component-args"][9]);

			m_Scene->AddComponent<Transform>(objectID, position, rotation, scale);
		}
		else if(componentData["component-type"] == "MeshComponent")
		{
			std::shared_ptr<Luna::IMesh> mesh = assetManager.GetMesh(componentData["component-args"][0].get<std::string>());
			std::shared_ptr<Luna::IShader> shader = assetManager.GetShader(componentData["component-args"][1].get<std::string>());
			std::shared_ptr<Luna::ITexture> texture = assetManager.GetTexture(componentData["component-args"][2].get<std::string>());

			m_Scene->AddComponent<MeshComponent>(objectID, mesh.get(), shader.get(), nullptr, texture.get(), texture.get());
		}
		else if (componentData["component-type"] == "CameraComponent")
		{
			glm::vec4 backgroundColor = glm::vec4(
				componentData["component-args"][0],
				componentData["component-args"][1],
				componentData["component-args"][2],
				componentData["component-args"][3]);

			m_Scene->AddComponent<CameraComponent>(objectID, new PerspectiveCamera(), backgroundColor, componentData["component-args"][4]);
			m_Scene->GetECS()->GetObjectComponent<CameraComponent>(objectID)->m_Camera->m_EditorBackgroundColour = backgroundColor;
		}
		else if (componentData["component-type"] == "LightComponent")
		{
			glm::vec3 lightColour = glm::vec3(componentData["component-args"][0], componentData["component-args"][1], componentData["component-args"][2]);
			m_Scene->AddComponent<LightComponent>(objectID, Luna::Light());
		}
		else if (componentData["component-type"] == "ScriptComponent")
		{
			std::shared_ptr<Script> script = assetManager.GetScript(componentData["component-args"][0].get<std::string>());

			m_Scene->AddComponent<ScriptComponent>(objectID, script);
			m_Scene->GetECS()->GetObjectComponent<ScriptComponent>(objectID)->m_Script->m_ECS = m_Scene->GetECS();
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

unsigned int SceneManager::AddObject(unsigned int parent, std::string objectName)
{
	unsigned int object = m_Scene->AddObject(parent);
	m_Scene->AddComponent<NameComponent>(object, objectName);
	m_Scene->AddComponent<Transform>(object); 
	return object;
}