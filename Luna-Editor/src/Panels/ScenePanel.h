#pragma once

#include "ImGuiPanel.h"
#include "../imgui/ImGuizmo.h"
#include "GLM/gtx/matrix_decompose.hpp"
#include "../EditorCamera.h"

class ScenePanel : public ImGuiPanel
{
public:
	ScenePanel(SceneManager* sceneManager = nullptr) : ImGuiPanel(sceneManager) {}

	// MUST BOTH BE CALLED
	void UpdateScene(unsigned int& inspectorID, FrameBuffer* framebuffer, std::vector<std::pair<ACTIONS, std::string>>* actions);
	void UpdateGizmos(unsigned int& inspectorID, ObjectTransformPairing<Camera>& camera);

private:
	ImGuizmo::OPERATION m_CurrentOperation = ImGuizmo::TRANSLATE;
};

