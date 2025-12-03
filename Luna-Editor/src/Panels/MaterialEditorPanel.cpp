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
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::SetCursorPos(pos);
		if(m_Material != nullptr)
		{
			ImGui::Text(m_Material->m_Path.c_str());
		}
		else
		{
			ImGui::Text("Drag and drop a material asset to edit a material");
		}

		ImGui::End();
	}
}