#pragma once

#include "ImGuiPanel.h"
#include "GLM/glm.hpp"
#include "GLM/gtc/type_ptr.hpp"

#include "Luna.h"

class InspectorPanel : public ImGuiPanel
{
public:
	InspectorPanel(SceneManager* sceneManager = nullptr) : ImGuiPanel(sceneManager) 
	{
		if(sceneManager == nullptr) return;

		m_ModelIcon = sceneManager->GetAssetManager()->GetTexture(false, "Assets/Textures/model.png");
		m_CodeIcon = sceneManager->GetAssetManager()->GetTexture(false, "Assets/Textures/web-programming.png");
	}

	void Update(unsigned int& inspectorID) override;
private:
	glm::vec3 m_EulerDegrees;

	std::shared_ptr<Luna::ITexture> m_ModelIcon = nullptr;
	std::shared_ptr<Luna::ITexture> m_CodeIcon = nullptr;
};

