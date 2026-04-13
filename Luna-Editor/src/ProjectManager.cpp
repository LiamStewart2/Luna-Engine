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

std::string ProjectManager::NavigateFolders()
{
	return FileNavigation::OpenFileDialog({
					{L"Project Files", L"*.luna"},
					{L"All Files", L"*.*"}
		}, 1);
}