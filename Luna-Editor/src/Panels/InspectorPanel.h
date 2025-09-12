#pragma once

#include "ImGuiPanel.h"

class InspectorPanel : public ImGuiPanel
{
public:
	InspectorPanel(SceneManager* sceneManager = nullptr) : ImGuiPanel(sceneManager) {}

	void Update(unsigned int& inspectorID) override;
};

