#include "ContentBrowserPanel.h"

void ContentBrowserPanel::Update(unsigned int& inspectorID)
{
	ImGui::Begin("Content Browser");

	if (m_CurrentDirectory != m_ProjectDirectory)
	{
		if(ImGui::Button("<-"))
			m_CurrentDirectory = m_CurrentDirectory.parent_path();
		ImGui::SameLine();
	}
	ImGui::Text(m_CurrentDirectory.string().c_str());

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
		ImGui::PushID(path.path().filename().string().c_str());

		if (GetFileExtension(path.path().filename().string()) == "png" || GetFileExtension(path.path().filename().string()) == "jpg" || GetFileExtension(path.path().filename().string()) == "jpeg")
			ImGui::Image(m_SceneManager->GetAssetManager()->GetTexture(path.path().string()).get()->ID, { thumbnailSize, thumbnailSize });
		else if(GetFileExtension(path.path().filename().string()) == "obj")
			ImGui::Image(m_ModelIcon->ID, { thumbnailSize, thumbnailSize });
		else
			ImGui::Image(m_FileIcon.get()->ID, { thumbnailSize, thumbnailSize });

		std::string itemPath = std::filesystem::relative(path.path(), m_ProjectDirectory).string().c_str();
		if (GetFileExtension(path.path().filename().string()) == "json")
			BeginPayload("CONTENT_BROWSER_ITEM_SCENE", itemPath, m_FileIcon.get()->ID, thumbnailSize);

		else if (GetFileExtension(path.path().filename().string()) == "png" || GetFileExtension(path.path().filename().string()) == "jpg" || GetFileExtension(path.path().filename().string()) == "jpeg")
			BeginPayload("CONTENT_BROWSER_ITEM_TEXTURE", itemPath, m_SceneManager->GetAssetManager()->GetTexture(path.path().string()).get()->ID, thumbnailSize);

		else if (GetFileExtension(path.path().filename().string()) == "obj")
			BeginPayload("CONTENT_BROWSER_ITEM_MODEL", itemPath, m_ModelIcon.get()->ID, thumbnailSize);

		else if (GetFileExtension(path.path().filename().string()) == "lua")
			BeginPayload("CONTENT_BROWSER_ITEM_SCRIPT", itemPath, m_FileIcon.get()->ID, thumbnailSize);

		ImGui::TextWrapped(path.path().filename().string().c_str());

		ImGui::NextColumn();

		ImGui::PopID();
	}

	ImGui::End();
}

std::string ContentBrowserPanel::GetFileExtension(std::string filename)
{
	size_t extension = filename.find(".");
	return filename.substr(extension + 1);
}


void ContentBrowserPanel::BeginPayload(std::string payloadID, std::string data, unsigned int textureID, float thumbnailSize)
{
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
	{
		ImGui::Image(textureID, { thumbnailSize, thumbnailSize });

		ImGui::SetDragDropPayload(payloadID.c_str(), data.c_str(), data.size() + 1);
		ImGui::EndDragDropSource();
	}
}