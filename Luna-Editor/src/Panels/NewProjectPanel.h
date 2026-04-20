#pragma once
#include "ImGuiPanel.h"
#include "../ProjectManager.h"
#include "ContentBrowserPanel.h"
#include <string>

class NewProjectPanel : public ImGuiPanel
{
public:
	NewProjectPanel(SceneManager* sceneManager = nullptr, ProjectManager* projectManager = nullptr) : ImGuiPanel(sceneManager), m_ProjectManager(projectManager) 
	{
		if(sceneManager == nullptr) return;
		m_FolderIcon = sceneManager->GetAssetManager()->GetTexture(false, "Assets/Textures/open-folder.png");
	}

	void Update(ContentBrowserPanel* contentBrowser, std::vector<std::pair<ACTIONS, std::string>>* actions);

private:
	ProjectManager* m_ProjectManager = nullptr;

	std::string m_ProjectNameBuffer;
	std::string m_ProjectFilepathBuffer;

	std::shared_ptr<Luna::ITexture> m_FolderIcon;
};

