#pragma once

#include "ImGuiPanel.h"

class GamePanel : public ImGuiPanel
{
public:
	GamePanel(SceneManager* sceneManager = nullptr) : ImGuiPanel(sceneManager) {}

	void UpdateGame(unsigned int& inspectorID, FrameBuffer* framebuffer);
};

