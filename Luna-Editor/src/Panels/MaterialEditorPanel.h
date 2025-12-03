#pragma once

#include "ImGuiPanel.h"

#include "Luna.h"

#include <string>

class MaterialEditorPanel : public ImGuiPanel
{
public:
	MaterialEditorPanel(SceneManager* sceneManager = nullptr) : ImGuiPanel(sceneManager)
	{
		if(sceneManager == nullptr) return;
	}

	void Update(unsigned int& inspectorID) override;
private:
	std::shared_ptr<Luna::Material> m_Material = nullptr;
};

