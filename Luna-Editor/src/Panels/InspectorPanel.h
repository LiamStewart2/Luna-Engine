#pragma once

#include "ImGuiPanel.h"

#include "Luna.h"

class InspectorPanel : public ImGuiPanel
{
public:
	InspectorPanel(SceneManager* sceneManager = nullptr) : ImGuiPanel(sceneManager) 
	{
		if(sceneManager == nullptr) return;

		m_ModelIcon = sceneManager->GetAssetManager()->GetTexture("Assets/Textures/model.png").get();
		m_CodeIcon = sceneManager->GetAssetManager()->GetTexture("Assets/Textures/web-programming.png").get();
	}

	void Update(unsigned int& inspectorID) override;
private:
	glm::vec3 m_EulerDegrees;

	Texture* m_ModelIcon = nullptr;
	Texture* m_CodeIcon = nullptr;
};

