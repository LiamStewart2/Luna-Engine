#include "MaterialEditorPanel.h"

void MaterialEditorPanel::Update(unsigned int& inspectorID)
{
	if(m_Show == false)
		return;
	{
		ImGui::Begin("Material Editor", &m_Show);

		ImGui::End();
	}
}