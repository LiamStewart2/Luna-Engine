#include "ShaderGraphEditorPanel.h"


ShaderGraphEditorPanel::ShaderGraphEditorPanel(SceneManager* sceneManager)
{
	m_CurrentShaderGraph = new Luna::ShaderGraph();

	m_CurrentShaderGraph->SpawnNode(new Luna::OutputNode(0, glm::vec2(100, 100)));
	m_CurrentShaderGraph->SpawnNode(new Luna::OutputNode(0, glm::vec2(200, 100)));



	delegate.LoadNodes(m_CurrentShaderGraph);

	fit = GraphEditor::Fit_AllNodes;
}

void ShaderGraphEditorPanel::Update(unsigned int& inspectorID)
{
	if (m_Show == false)
		return;
	{
		ImGui::Begin("Shader Graph", &m_Show);

		GraphEditor::Show(delegate, options, viewState, true, &fit);

		ImGui::End();
	}
}