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

	// Folder Contents UI
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
		std::string str = path.path().string();
		if (ImGui::ImageButton(path.path().string().c_str(), m_FolderIcon->GetTextureReference(), {thumbnailSize, thumbnailSize}))
		{
			m_CurrentDirectory = path.path();
			break;
		}
		if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
		{
			m_RightClickedPath = path.path().filename().string();
		}
		ImGui::TextWrapped(path.path().filename().string().c_str());
		ImGui::NextColumn();
	}

	for (auto& path : sortedFiles)
	{
		ImGui::PushID(path.path().filename().string().c_str());

		if (GetFileExtension(path.path().filename().string()) == "png" || GetFileExtension(path.path().filename().string()) == "jpg" || GetFileExtension(path.path().filename().string()) == "jpeg")
			ImGui::Image(m_SceneManager->GetAssetManager()->GetTexture(true, path.path().string())->GetTextureReference(), { thumbnailSize, thumbnailSize });
		else if(GetFileExtension(path.path().filename().string()) == "obj")
			ImGui::Image(m_ModelIcon->GetTextureReference(), { thumbnailSize, thumbnailSize });
		else
			ImGui::Image(m_FileIcon->GetTextureReference(), { thumbnailSize, thumbnailSize });

		if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
		{
			m_RightClickedPath = path.path().filename().string();
		}

		// ToDo: make this use relative pathing to allow the project to move without creating issues
		//std::string itemPath = std::filesystem::relative(path.path(), m_ProjectDirectory).string().c_str();
		std::string itemPath = path.path().string();

		if (GetFileExtension(path.path().filename().string()) == "json")
			BeginPayload("CONTENT_BROWSER_ITEM_SCENE", itemPath, m_FileIcon->GetTextureReference(), thumbnailSize);

		else if (GetFileExtension(path.path().filename().string()) == "png" || GetFileExtension(path.path().filename().string()) == "jpg" || GetFileExtension(path.path().filename().string()) == "jpeg")
			BeginPayload("CONTENT_BROWSER_ITEM_TEXTURE", itemPath, m_SceneManager->GetAssetManager()->GetTexture(true, path.path().string())->GetTextureReference(), thumbnailSize);

		else if (GetFileExtension(path.path().filename().string()) == "obj")
			BeginPayload("CONTENT_BROWSER_ITEM_MODEL", itemPath, m_ModelIcon->GetTextureReference(), thumbnailSize);

		else if (GetFileExtension(path.path().filename().string()) == "lua")
			BeginPayload("CONTENT_BROWSER_ITEM_SCRIPT", itemPath, m_FileIcon->GetTextureReference(), thumbnailSize);

		else if (GetFileExtension(path.path().filename().string()) == "lmat")
			BeginPayload("CONTENT_BROWSER_ITEM_MATERIAL", itemPath, m_FileIcon->GetTextureReference(), thumbnailSize);


		ImGui::TextWrapped(path.path().filename().string().c_str());

		ImGui::NextColumn();

		ImGui::PopID();
	}

	// Add assets menu
	if (ImGui::BeginPopupContextWindow("ContentBrowsewContextWindow", ImGuiPopupFlags_MouseButtonRight))
	{
		m_ContextWindowOpen = true;
		if (m_RightClickedPath != "")
		{
			if (ImGui::MenuItem("Delete"))
			{
				DeleteSelectedFileOrFolder();
			}
		}
		if (ImGui::BeginMenu("Create Asset"))
		{
			ImGui::SeparatorText("Assets");
			if (ImGui::MenuItem("Material"))
			{
				CreateNewMaterial();
			}
			ImGui::EndMenu();
		}

		ImGui::EndPopup();
	}
	else
	{
		if(m_ContextWindowOpen)
		{
			m_RightClickedPath = "";
			m_ContextWindowOpen = false;
		}
	}
	ImGui::End();
}

std::string ContentBrowserPanel::GetFileExtension(std::string filename)
{
	size_t extension = filename.find(".");
	return filename.substr(extension + 1);
}


void ContentBrowserPanel::BeginPayload(std::string payloadID, std::string data, void* textureID, float thumbnailSize)
{
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
	{
		ImGui::Image(textureID, { thumbnailSize, thumbnailSize });

		ImGui::SetDragDropPayload(payloadID.c_str(), data.c_str(), data.size() + 1);
		ImGui::EndDragDropSource();
	}
}

void ContentBrowserPanel::DeleteSelectedFileOrFolder()
{
	std::filesystem::path target = m_CurrentDirectory / m_RightClickedPath;

	if(!std::filesystem::exists(target))
		std::cerr << target.string() << ">>> Path not found" << std::endl;
	else 
	{
		if (std::filesystem::is_directory(target))
			std::filesystem::remove_all(target);
		else
			std::filesystem::remove(target);

		m_RightClickedPath = "";
	}
}

void ContentBrowserPanel::CreateNewMaterial()
{
	std::string path = m_CurrentDirectory.string() + "/new_material.lmat";

	nlohmann::json jsonData;
	jsonData["Textures"]["Albedo"] = "Assets/Textures/default.png";
	jsonData["Textures"]["SpecularMap"] = "Assets/Textures/default.png";
	jsonData["Textures"]["NormalMap"] = "Assets/Textures/BumpMapFlatColour.png";
	jsonData["Textures"]["MetallicMap"] = "Assets/Textures/NonMetallic.png";
	jsonData["Textures"]["AOMap"] = "Assets/Textures/default.png";

	glm::vec4 color = glm::vec4(1, 1, 1, 1);
	jsonData["Values"]["AmbientColour"] = { color.x, color.y, color.z, color.a };
	jsonData["Values"]["AmbientIntensity"] = 0.2;

	color = glm::vec4(1, 1, 1, 1);
	jsonData["Values"]["SpecularColour"] = { color.x, color.y, color.z, color.a };
	jsonData["Values"]["SpecularIntensity"] = 0.8;

	std::ofstream file(path);
	file << jsonData.dump(4);
}


void ContentBrowserPanel::ResetDirectory(std::string workingDirectory)
{
	m_CurrentDirectory = workingDirectory;
	m_ProjectDirectory = workingDirectory;
}