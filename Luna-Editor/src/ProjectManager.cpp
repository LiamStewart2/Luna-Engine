#include "ProjectManager.h"


void ProjectManager::OpenProject(SceneManager* sceneManager, std::string filepath)
{
	std::ifstream file(filepath);
	nlohmann::json jsonData = nlohmann::json::parse(file);

	std::filesystem::path path(filepath);
	std::filesystem::path projectDir = path.parent_path();
	std::filesystem::path workingDir = projectDir / "Assets";

	m_WorkingProject = {
		workingDir.string(),
		jsonData["ProjectName"],
		jsonData["DefaultScene"]
	};
	
	sceneManager->LoadNewScene(jsonData["DefaultScene"].get<std::string>().c_str());
}

void ProjectManager::OnEditorStart(SceneManager* sceneManager)
{
	std::string fpath = FileNavigation::OpenFileDialog({
					{L"Project Files", L"*.luna"},
					{L"All Files", L"*.*"}
		}, 1);
	if (!fpath.empty())
	{
		OpenProject(sceneManager, fpath);
	}
}