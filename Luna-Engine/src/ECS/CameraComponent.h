#pragma once

#include "GLM/glm.hpp"

#include "Component.h"
#include "../Core/Camera.h"

struct CameraComponent : public Component
{
	CameraComponent(unsigned int _gameObject = 0, Camera* camera = nullptr, bool useSkybox = false, glm::vec4 cameraBackgroundColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f), std::shared_ptr<Luna::ITexture> skyboxTexture = nullptr, bool _mainCamera = false) :
		Component(_gameObject), m_Camera(camera), m_MainCamera(_mainCamera), m_UseSkybox(useSkybox), m_CameraBackgroundColor(cameraBackgroundColor), m_SkyboxTexture(skyboxTexture)
	{
	}

	Camera* m_Camera;
	bool m_MainCamera = false;

	bool m_UseSkybox = false;
	glm::vec4 m_CameraBackgroundColor = glm::vec4(1);
	std::shared_ptr<Luna::ITexture> m_SkyboxTexture = nullptr;
};