#include "ShaderGraphEditorPanel.h"


void ShaderGraphEditorPanel::Update(unsigned int& inspectorID)
{
	if (m_Show == false)
		return;
	{
		ImGui::Begin("Shader Graph", &m_Show);

		ImGui::Text("Fortnite");

		ImGui::End();
	}
}