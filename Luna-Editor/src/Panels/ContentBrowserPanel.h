#pragma once

#include "ImGuiPanel.h"
#include "Luna.h"

#include <filesystem>

class ContentBrowserPanel : public ImGuiPanel
{
public:
	ContentBrowserPanel(SceneManager* sceneManager = nullptr, std::filesystem::path projectDirectory = "")
		: ImGuiPanel(sceneManager), m_ProjectDirectory(projectDirectory), m_CurrentDirectory(projectDirectory) 
	{
		if(sceneManager == nullptr) return;

		m_FolderIcon = sceneManager->GetAssetManager()->GetTexture("Assets/Textures/open-folder.png");
		m_FileIcon = sceneManager->GetAssetManager()->GetTexture("Assets/Textures/document.png");
	}

	void Update(unsigned int& inspectorID) override;

private:
	std::filesystem::path m_ProjectDirectory;
	std::filesystem::path m_CurrentDirectory;

	std::shared_ptr<Texture> m_FolderIcon;
	std::shared_ptr<Texture> m_FileIcon;
};

