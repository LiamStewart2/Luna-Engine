#pragma once

#include "ImGuiPanel.h"

class HierarchyPanel : public ImGuiPanel
{
public:
	HierarchyPanel(SceneManager* sceneManager = nullptr) : ImGuiPanel(sceneManager) {}

	void Update(unsigned int& inspectorID) override;
	void EndFrame(unsigned int& inspectorID) override;

private:
	void BuildHiearchyText(unsigned int& inspectorID, SceneGraphNode* node, std::unordered_map<unsigned int, NameComponent>& names, bool& HasBeenRightClicked);

	std::vector<unsigned int> m_ObjectsToDelete;
	std::vector<std::pair<unsigned int, std::string>> m_ObjectsToAdd;
};

