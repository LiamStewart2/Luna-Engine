#include "MaterialEditorPanel.h"

void MaterialEditorPanel::Update(unsigned int& inspectorID)
{
	if(m_Show == false)
		return;
	{
		ImGui::Begin("Material Editor", &m_Show);
		Scene* scene = m_SceneManager->GetCurrentScene();

		ImVec2 pos = ImGui::GetCursorPos();
		ImGui::Dummy(ImGui::GetContentRegionAvail());

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM_MATERIAL"))
			{
				const char* path = (const char*)payload->Data;

				m_Material = m_SceneManager->GetAssetManager()->GetMaterial(true, path);

                delegate.textures = std::vector<std::shared_ptr<Luna::ITexture>>({
                    m_Material->m_Albedo, m_Material->m_SpecularMap, m_Material->m_NormalMap, m_Material->m_MetallicMap, m_Material->m_AOMap});
				delegate.material = m_Material;
			}
			ImGui::EndDragDropTarget();
		}

		

		ImGui::SetCursorPos(pos);
		if(m_Material != nullptr)
		{   
			std::string materialPath = m_Material->m_Path;
			if(delegate.changesMade)
				materialPath += "*";

            ImGui::Text(materialPath.c_str()); const int spacing = 250;
			ImGui::SameLine();

			if (ImGui::Button("Save Material"))
				SaveCurrentMaterial();
            ImGui::SameLine(ImGui::GetWindowWidth() - spacing);

            if (ImGui::Button("Fit all nodes"))
                fit = GraphEditor::Fit_AllNodes;
            ImGui::SameLine();

            if (ImGui::Button("Fit selected nodes"))
                fit = GraphEditor::Fit_SelectedNodes;

            GraphEditor::Show(delegate, options, viewState, true, &fit);

		}
		else
		{
			ImGui::Text("Drag and drop a material asset to edit a material");
		}

		ImGui::End();
	}
}

void MaterialEditorPanel::SaveCurrentMaterial()
{
	if(m_Material == nullptr)
		return;

	std::string path = m_Material->m_Path;

	nlohmann::json jsonData;
	jsonData["Textures"]["Albedo"] = m_Material->m_Albedo->GetTexturePacket()->path;
	jsonData["Textures"]["SpecularMap"] = m_Material->m_SpecularMap->GetTexturePacket()->path;
	jsonData["Textures"]["NormalMap"] = m_Material->m_NormalMap->GetTexturePacket()->path;
	jsonData["Textures"]["MetallicMap"] = m_Material->m_MetallicMap->GetTexturePacket()->path;
	jsonData["Textures"]["AOMap"] = m_Material->m_AOMap->GetTexturePacket()->path;

	glm::vec4 color = m_Material->m_AmbientColour;
	jsonData["Values"]["AmbientColour"] = {color.x, color.y, color.z, color.a};
	jsonData["Values"]["AmbientIntensity"] = m_Material->m_AmbientIntensity;

	color = m_Material->m_SpecularColour;
	jsonData["Values"]["SpecularColour"] = { color.x, color.y, color.z, color.a };
	jsonData["Values"]["SpecularIntensity"] = m_Material->m_SpecularIntensity;

	std::ofstream file(path);
	file << jsonData.dump(4);

	delegate.changesMade = false;

	std::cout << "Serialized material: " << path << std::endl;
}