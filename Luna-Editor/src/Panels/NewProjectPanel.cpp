#include "NewProjectPanel.h"
#include <filesystem>

void NewProjectPanel::Update(ContentBrowserPanel* contentBrowser, std::vector<std::pair<ACTIONS, std::string>>* actions)
{
	if(m_Show == false)
		return;

	{
		ImGui::Begin("New Project", &m_Show);

		ImGui::Text("Create a new project");
		
		ImGui::InputText("Project Name", &m_ProjectNameBuffer);
		ImGui::InputText("Project Path", &m_ProjectFilepathBuffer);

		ImGui::SameLine();
		if (ImGui::ImageButton("ProjectPathButton", m_FolderIcon->GetTextureReference(), {20, 20}))
		{
			std::string fpath = FileNavigation::BrowseDialog();
			if (!fpath.empty())
			{
				m_ProjectFilepathBuffer = fpath.c_str();
			}
		}

		if (ImGui::Button("Create New Project"))
		{

			std::filesystem::path p = m_ProjectFilepathBuffer;
			std::filesystem::path project_path = p / m_ProjectNameBuffer;
			std::filesystem::path projectfile_path = project_path / (m_ProjectNameBuffer + ".luna");

			Project project = Project({
				project_path.string(),
				m_ProjectNameBuffer,
				"Assets\\Template Scene.json"
			});

			m_ProjectManager->CreateNewProject(m_SceneManager, project);

			m_ProjectManager->OpenProject(projectfile_path.string());
			m_SceneManager->SetAssetWorkingPath(m_ProjectManager->GetOpenProject().m_WorkingDirectory);
			contentBrowser->ResetDirectory(m_ProjectManager->GetOpenProject().m_WorkingDirectory);

			actions->push_back({ LOADSCENE, m_ProjectManager->GetOpenProject().m_DefaultScenePath });
			Close();
		}

		ImGui::End();
	}
}