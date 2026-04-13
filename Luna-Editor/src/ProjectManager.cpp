#include "ProjectManager.h"


void ProjectManager::OpenProject(std::string filepath)
{
	std::ifstream file(filepath);
	nlohmann::json jsonData = nlohmann::json::parse(file);

	std::filesystem::path path(filepath);
	std::filesystem::path projectDir = path.parent_path();
	std::filesystem::path workingDir = projectDir;

	m_WorkingProject = {
		workingDir.string(),
		jsonData["ProjectName"],
		jsonData["DefaultScene"]
	};
}

void ProjectManager::OnEditorStart(SceneManager* sceneManager)
{
	std::string fpath = NavigateFolders();
	if (!fpath.empty())
	{
		OpenProject(fpath);
		sceneManager->SetAssetWorkingPath(GetOpenProject().m_WorkingDirectory);
		sceneManager->LoadNewScene(m_WorkingProject.m_DefaultScenePath.c_str());
	}
}

void ProjectManager::CreateNewProject(SceneManager* sceneManager, const Project& project)
{
	// Create the project file:
	nlohmann::json jsonData;

	jsonData["ProjectName"] = project.m_ProjectName;
	jsonData["DefaultScene"] = project.m_DefaultScenePath;

	std::filesystem::path folderPath(project.m_WorkingDirectory);
	std::filesystem::path path = folderPath / (project.m_ProjectName + ".luna");


	std::filesystem::create_directories(std::filesystem::path(path).parent_path());
	std::ofstream file(path.string());
	file << jsonData.dump(4);

	// Create the default scene file
	Scene virtualScene = Scene();
	virtualScene.Init(sceneManager->GetAssetManager(), "Template Scene");
	
	unsigned int camera = virtualScene.AddObject();
	unsigned int light = virtualScene.AddObject();
	unsigned int floor = virtualScene.AddObject();

	/* Camera Components */
	virtualScene.AddComponent<NameComponent>(camera, "Camera");
	virtualScene.AddComponent<Transform>(camera, glm::vec3(0, 0, -5));
	virtualScene.AddComponent<CameraComponent>(camera, nullptr, true, glm::vec4(1, 1, 1, 1), true);
	
	/* Light Components */
	virtualScene.AddComponent<NameComponent>(light, "Light");
	virtualScene.AddComponent<Transform>( light,
		glm::vec3(0, 4, 6.2),
		glm::quat(1, -0.2, 0.35, 0));
	virtualScene.AddComponent<LightComponent>(light, Luna::Light(Luna::LightType::Directional, glm::vec4(1)));

	/* Floor Components */
	Luna::AssetManager* assetManager = sceneManager->GetAssetManager();

	virtualScene.AddComponent<NameComponent>(floor, "floor");
	virtualScene.AddComponent<Transform>(floor, glm::vec3(0, 0, 0), glm::quat(1, 0, 0, 0), glm::vec3(5, 1, 5));

	/* Serialize */
	std::filesystem::path scenePath = folderPath / project.m_DefaultScenePath;
	sceneManager->SaveScene(&virtualScene, scenePath.string());
}

std::string ProjectManager::NavigateFolders()
{
	return FileNavigation::OpenFileDialog({
					{L"Project Files", L"*.luna"},
					{L"All Files", L"*.*"}
		}, 1);
}