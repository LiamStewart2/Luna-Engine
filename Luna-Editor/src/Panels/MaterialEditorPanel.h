#pragma once

#include "ImGuiPanel.h"

#include "Luna.h"

class MaterialEditorPanel : public ImGuiPanel
{
public:
	MaterialEditorPanel(SceneManager* sceneManager = nullptr) : ImGuiPanel(sceneManager)
	{
		if(sceneManager == nullptr) return;
	}

	void Update(unsigned int& inspectorID) override;
private:

};

