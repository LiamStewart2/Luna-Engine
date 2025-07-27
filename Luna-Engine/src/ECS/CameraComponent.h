#pragma once

#include "GLM/glm.hpp"

#include "Component.h"

class CameraComponent : public Component
{
	CameraComponent(unsigned int _gameObject = 0, glm::vec3 _up = glm::vec3(0), bool _renderToScreen = true) :
		Component(_gameObject), m_Up(_up), m_RenderToScreen(_renderToScreen) {}

	glm::vec3 m_Up;
	bool m_RenderToScreen;
};