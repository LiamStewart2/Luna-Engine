#pragma once

#include "GLM/glm.hpp"

#include "Component.h"
#include "../Renderer/Light.h"

struct LightComponent : public Component
{
	LightComponent(unsigned int _gameObject = 0, glm::vec3 lightColor = glm::vec3(1, 1, 1)) : 
		Component(_gameObject), m_LightColor(lightColor) {}

	Light m_Light;
	glm::vec3 m_LightColor;
};