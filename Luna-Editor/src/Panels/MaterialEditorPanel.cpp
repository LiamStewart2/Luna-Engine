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

				m_Material = m_SceneManager->GetAssetManager()->GetMaterial(path);

                delegate.textures = std::vector<std::shared_ptr<Luna::ITexture>>({
                    m_Material->m_Albedo, m_Material->m_SpecularMap, m_Material->m_NormalMap});
				delegate.material = m_Material;
			}
			ImGui::EndDragDropTarget();
		}

		

		ImGui::SetCursorPos(pos);
		if(m_Material != nullptr)
		{   
            ImGui::Text(m_Material->m_Path.c_str()); const int spacing = 250;
			ImGui::SameLine();
			if (ImGui::Button("Save Material"))
			{

			}
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