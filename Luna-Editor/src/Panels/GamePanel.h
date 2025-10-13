#pragma once

#include "ImGuiPanel.h"

#include "Luna.h"

class GamePanel : public ImGuiPanel
{
public:
	GamePanel(SceneManager* sceneManager = nullptr) : ImGuiPanel(sceneManager) {
		if (sceneManager == nullptr) return;

		m_PlayIcon = sceneManager->GetAssetManager()->GetTexture("Assets/Textures/play-button.png");
		m_StopIcon = sceneManager->GetAssetManager()->GetTexture("Assets/Textures/stop.png");
	}

	void UpdateGame(unsigned int& inspectorID, bool& runtime);
private:
	std::shared_ptr<Texture> m_PlayIcon = nullptr;
	std::shared_ptr<Texture> m_StopIcon = nullptr;
};

