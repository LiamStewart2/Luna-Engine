#include "SceneManager.h"


SceneManager::SceneManager()
{

}

SceneManager::~SceneManager()
{
}

void SceneManager::InitFramebuffer()
{
	m_FrameBuffer = FrameBuffer(FramebufferSpecification(1920, 1080, std::vector<FramebufferTextureAttatchment>({ RGBA8, DEPTH })));
	m_FrameBuffer.Update();
}

void SceneManager::Update()
{
	m_Scene->Update();
}

void SceneManager::Render(Renderer* renderer)
{
	m_Scene->Render(renderer, &m_FrameBuffer);
}

void SceneManager::SaveScene()
{
	if (m_Scene == nullptr)
		return;

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
				{"component-args", {component->mesh->path, component->shader->path, component->texture->path}}});
		}

		if (m_Scene->GetECS()->HasComponent<CameraComponent>(m_Scene->GetGameObjects()->at(i)))
		{
			CameraComponent* component = m_Scene->GetECS()->GetObjectComponent<CameraComponent>(m_Scene->GetGameObjects()->at(i));
			objectComponents.push_back({
				{"component-type", "CameraComponent"},
				{"component-args", {component->m_MainCamera}}
			});
		}

		if (m_Scene->GetECS()->HasComponent<LightComponent>(m_Scene->GetGameObjects()->at(i)))
		{
			LightComponent* component = m_Scene->GetECS()->GetObjectComponent<LightComponent>(m_Scene->GetGameObjects()->at(i));
			objectComponents.push_back({
				{"component-type", "LightComponent"},
				{"component-args", {component->m_LightColor.x, component->m_LightColor.y, component->m_LightColor.z}}
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

	// Load the scene file using json :: ToDo change json to a more suitable format
	std::ifstream file(filepath);
	nlohmann::json jsonData = nlohmann::json::parse(file);

	std::cout << "Loading Scene -- Scene name: " << jsonData["scene-name"] << std::endl;

	m_Scene = new Scene();
	m_Scene->filepath = std::string(filepath);
	m_Scene->Init(&assetManager, &lightManager, jsonData["scene-name"]);

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
			std::shared_ptr<Mesh> mesh = assetManager.GetMesh(componentData["component-args"][0].get<std::string>());
			std::shared_ptr<Shader> shader = assetManager.GetShader(componentData["component-args"][1].get<std::string>());
			std::shared_ptr<Texture> texture = assetManager.GetTexture(componentData["component-args"][2].get<std::string>());

			m_Scene->AddComponent<MeshComponent>(objectID, mesh.get(), shader.get(), &defaultMat, texture.get());
		}
		else if (componentData["component-type"] == "CameraComponent")
		{
			m_Scene->AddComponent<CameraComponent>(objectID, new PerspectiveCamera(), componentData["component-args"][0]);

			lightManager.InitCascadeLevels(m_Scene->GetECS()->GetObjectComponent<CameraComponent>(objectID)->m_Camera);
		}
		else if (componentData["component-type"] == "LightComponent")
		{
			glm::vec3 lightColour = glm::vec3(componentData["component-args"][0], componentData["component-args"][1], componentData["component-args"][2]);
			m_Scene->AddComponent<LightComponent>(objectID, lightColour);
			m_Scene->GetECS()->GetObjectComponent<LightComponent>(objectID)->m_Light.BuildLight(&lightManager);
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