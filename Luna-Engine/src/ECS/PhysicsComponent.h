#pragma once

#include <string>
#include <GLM/glm.hpp>

#include "Component.h"

struct PhysicsComponent : public Component
{
	PhysicsComponent(unsigned int _gameObject = 0, bool simulate = true, float mass = 1.0f, float gravityValue = 9.81f) :
		Component(_gameObject), m_Simulate(simulate), m_Mass(mass), m_GravityValue(gravityValue)
	{
		
	}

	bool m_Simulate = true;
	bool m_BeingManipulated = false;

	float m_Mass = 1.0f;
	float m_GravityValue = 9.81f;

	glm::vec3 m_Velocity = glm::vec3(0, 0, 0);
};