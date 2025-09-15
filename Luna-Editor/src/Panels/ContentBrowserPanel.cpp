#include "ContentBrowserPanel.h"

void ContentBrowserPanel::Update(unsigned int& inspectorID)
{
	ImGui::Begin("Content Browser");

	if (m_CurrentDirectory != m_ProjectDirectory)
	{
		if(ImGui::Button("<-"))
			m_CurrentDirectory = m_CurrentDirectory.parent_path();
	}

	static float padding = 16.0f;
	static float thumbnailSize = 96.0f;
	float cellSize = thumbnailSize + padding;

	float panelWidth = ImGui::GetContentRegionAvail().x;
	int columnCount = std::floor(panelWidth / cellSize);
	

	std::set<std::filesystem::directory_entry> sortedDirectories;
	std::set<std::filesystem::directory_entry> sortedFiles;
	for (auto& path : std::filesystem::directory_iterator(m_CurrentDirectory))
	{
		if(path.is_directory())
			sortedDirectories.insert(path);
		else
			sortedFiles.insert(path);
	}

	ImGui::Columns(columnCount, 0, false);

	for (auto& path : sortedDirectories)
	{
		if (ImGui::ImageButton(path.path().string().c_str(), m_FolderIcon.get()->ID, {thumbnailSize, thumbnailSize}))
		{
			m_CurrentDirectory = path.path();
			break;
		}
		ImGui::TextWrapped(path.path().filename().string().c_str());
		ImGui::NextColumn();
	}

	for (auto& path : sortedFiles)
	{
		ImGui::Image(m_FileIcon.get()->ID, { thumbnailSize, thumbnailSize });
		ImGui::TextWrapped(path.path().filename().string().c_str());

		ImGui::NextColumn();
	}

	ImGui::End();
}
