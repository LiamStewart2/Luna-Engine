#pragma once

#include "GLM/glm.hpp"

#include "Component.h"

struct CameraComponent : public Component
{
	CameraComponent(unsigned int _gameObject = 0, Camera* camera = nullptr, bool _mainCamera = false) :
		Component(_gameObject), m_Camera(camera), m_MainCamera(_mainCamera) {}

	Camera* m_Camera;
	bool m_MainCamera = false;
};