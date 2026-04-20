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

void SceneManager::SaveScene(Scene* scene, std::string path)
{
	if (path == "")		
		path = m_Scene->filepath;
	if(scene == nullptr)
		scene = m_Scene;
	SaveSceneAs(scene, path);
}

void SceneManager::SetAssetWorkingPath(std::string workingDir)
{
	m_WorkingDirectory = workingDir;
	assetManager.SetWorkingDirectory(workingDir);
}

void SceneManager::SaveSceneAs(Scene* scene, std::string path)
{
	if(scene == nullptr)
		return;

	nlohmann::json jsonData;
	
	jsonData["scene-name"] = scene->GetSceneName();

	// Save objects to json data

	jsonData["objects"] = nlohmann::json::array();

	for (int i = 0; i < scene->GetGameObjects()->size(); i++)
	{
		nlohmann::json objectComponents = nlohmann::json::array();

		if (scene->GetECS()->HasComponent<NameComponent>(scene->GetGameObjects()->at(i)))
		{
			NameComponent* component = scene->GetECS()->GetObjectComponent<NameComponent>(scene->GetGameObjects()->at(i));
			objectComponents.push_back({
				{"component-type", "NameComponent"},
				{"component-args", {component->m_Name}}
			});
		}

		if (scene->GetECS()->HasComponent<Transform>(scene->GetGameObjects()->at(i)))
		{
			Transform* component = scene->GetECS()->GetObjectComponent<Transform>(scene->GetGameObjects()->at(i));
			objectComponents.push_back({
				{"component-type", "TransformComponent"},
				{"component-args", {
					component->position.x, component->position.y, component->position.z,
					component->rotation.w, component->rotation.x, component->rotation.y, component->rotation.z,
					component->scale.x, component->scale.y, component->scale.z
				}}
			});
		}

		if (scene->GetECS()->HasComponent<MeshComponent>(scene->GetGameObjects()->at(i)))
		{
			MeshComponent* component = scene->GetECS()->GetObjectComponent<MeshComponent>(scene->GetGameObjects()->at(i));
			objectComponents.push_back({
				{"component-type", "MeshComponent"},
				{"component-args", {component->mesh->m_Path, component->shader->m_Path, component->material->m_Path}}});
		}

		if (scene->GetECS()->HasComponent<CameraComponent>(scene->GetGameObjects()->at(i)))
		{
			CameraComponent* component = scene->GetECS()->GetObjectComponent<CameraComponent>(scene->GetGameObjects()->at(i));
			objectComponents.push_back({
				{"component-type", "CameraComponent"},
				{"component-args", {
					component->m_CameraBackgroundColor.x, component->m_CameraBackgroundColor.y,
					component->m_CameraBackgroundColor.z, component->m_CameraBackgroundColor.w,
					component->m_MainCamera,
					component->m_UseSkybox}}
			});
		}

		if (scene->GetECS()->HasComponent<LightComponent>(scene->GetGameObjects()->at(i)))
		{
			LightComponent* component = scene->GetECS()->GetObjectComponent<LightComponent>(scene->GetGameObjects()->at(i));
			objectComponents.push_back({
				{"component-type", "LightComponent"},
				{"component-args", {component->m_Light.m_LightColour.x, component->m_Light.m_LightColour.y, component->m_Light.m_LightColour.z}}
			});
		}

		if (scene->GetECS()->HasComponent<ScriptComponent>(scene->GetGameObjects()->at(i)))
		{
			ScriptComponent* component = scene->GetECS()->GetObjectComponent<ScriptComponent>(scene->GetGameObjects()->at(i));
			objectComponents.push_back({
				{"component-type", "ScriptComponent"},
				{"component-args", {component->m_Script->GetFilepath()}}
			});
		}

		if (scene->GetECS()->HasComponent<PhysicsComponent>(scene->GetGameObjects()->at(i)))
		{
			PhysicsComponent* component = scene->GetECS()->GetObjectComponent<PhysicsComponent>(scene->GetGameObjects()->at(i));
			objectComponents.push_back({
				{"component-type", "PhysicsComponent"},
				{"component-args", {component->m_Simulate, component->m_Mass, component->m_GravityValue, component->m_Dynamic, component->m_Restitution}}
				});
		}

		if (scene->GetECS()->HasComponent<ColliderComponent>(scene->GetGameObjects()->at(i)))
		{
			ColliderComponent* component = scene->GetECS()->GetObjectComponent<ColliderComponent>(scene->GetGameObjects()->at(i));
			objectComponents.push_back({
				{"component-type", "ColliderComponent"},
				{"component-args", {component->m_Shape, component->m_ColliderSize.x, component->m_ColliderSize.y, component->m_ColliderSize.z}}
				});
		}

		jsonData["objects"].push_back(objectComponents);
	}
	
	// Load relations from scene graph
	jsonData["relations"] = nlohmann::json::array();
	SceneGraphNode* sceneGraph = scene->GetSceneGraph();

	SaveSceneNode(jsonData["relations"], sceneGraph);

	// Write to file
	if(path == "")
	{
		std::ofstream file(std::string(scene->GetSceneName()));
		file << jsonData.dump(4);
	}
	else
	{
		std::filesystem::create_directories(std::filesystem::path(path).parent_path());
		std::ofstream file(path);
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


	std::filesystem::path path(m_WorkingDirectory);
	std::filesystem::path SceneDirectory = path / filepath;


	std::ifstream file(SceneDirectory.string());
	nlohmann::json jsonData = nlohmann::json::parse(file);

	std::cout << "Loading Scene -- Scene name: " << jsonData["scene-name"] << std::endl;

	m_Scene = new Scene();
	m_Scene->filepath = SceneDirectory.string();
	m_Scene->Init(&assetManager, jsonData["scene-name"]);

	for(nlohmann::json data : jsonData["relations"])
		LoadRelations(jsonData, data, 0);

	m_Scene->InitAfterLoad();

	std::cout << "time to load " << jsonData["scene-name"] << " - " << glfwGetTime() - startTime << std::endl;
	
}

void SceneManager::LoadRelations(const nlohmann::json& originalData, const nlohmann::json& jsonData, unsigned int parentObjectID)
{

	unsigned int objectID = m_Scene->AddObject(parentObjectID);

	unsigned int index = jsonData["ObjectID"].get<unsigned int>();
	auto Components = originalData["objects"][index];

	//LoadComponents
	for (nlohmann::json componentData : Components)
	{
		try{
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
				std::shared_ptr<Luna::IMesh> mesh = assetManager.GetMesh(true, componentData["component-args"][0].get<std::string>());
				std::shared_ptr<Luna::IShader> shader = assetManager.GetShader(componentData["component-args"][1].get<std::string>());
				std::shared_ptr<Luna::Material> material = assetManager.GetMaterial(true, componentData["component-args"][2].get<std::string>());
				m_Scene->AddComponent<MeshComponent>(objectID, mesh.get(), shader.get(), material);
			}
			else if (componentData["component-type"] == "CameraComponent")
			{
				glm::vec4 backgroundColor = glm::vec4(
					componentData["component-args"][0],
					componentData["component-args"][1],
					componentData["component-args"][2],
					componentData["component-args"][3]);

				m_Scene->AddComponent<CameraComponent>(objectID, new PerspectiveCamera(), componentData["component-args"][5], backgroundColor,
					 componentData["component-args"][4]);
			}
			else if (componentData["component-type"] == "LightComponent")
			{
				glm::vec4 lightColour = glm::vec4(componentData["component-args"][0], componentData["component-args"][1], componentData["component-args"][2], 1);
				Luna::Light light = Luna::Light();
				light.m_Type = Luna::LightType::Directional;
				light.m_LightColour = lightColour;

				m_Scene->AddComponent<LightComponent>(objectID, light);
			}
			else if (componentData["component-type"] == "ScriptComponent")
			{
				std::shared_ptr<Script> script = assetManager.GetScript(true, componentData["component-args"][0].get<std::string>());

				m_Scene->AddComponent<ScriptComponent>(objectID, script);
				m_Scene->GetECS()->GetObjectComponent<ScriptComponent>(objectID)->m_Script->m_ECS = m_Scene->GetECS();
			}
			else if (componentData["component-type"] == "PhysicsComponent")
			{
				m_Scene->AddComponent<PhysicsComponent>(objectID, 
					componentData["component-args"][0].get<bool>(),
					componentData["component-args"][1].get<float>(),
					componentData["component-args"][2].get<float>(),
					componentData["component-args"][3].get<bool>(),
					componentData["component-args"][4].get<float>()
				);
			}
			else if (componentData["component-type"] == "ColliderComponent")
			{
				m_Scene->AddComponent<ColliderComponent>(objectID,
					componentData["component-args"][0].get<ColliderShape>(),
					glm::vec3(componentData["component-args"][1], componentData["component-args"][2], componentData["component-args"][3])
				);
			}
		}
		catch (...)
		{
			std::cout << "Failed to load component >>> " << componentData["component-type"] << std::endl;
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