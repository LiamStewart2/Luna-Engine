#pragma once

#include "GLM/glm.hpp"

#include "Component.h"
#include "../Renderer/Light.h"

struct LightComponent : public Component
{
	LightComponent(unsigned int _gameObject = 0, Luna::Light light = Luna::Light()) : 
		Component(_gameObject), m_Light(light){}

	Luna::Light m_Light;
};