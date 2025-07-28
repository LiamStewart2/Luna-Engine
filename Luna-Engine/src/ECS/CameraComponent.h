#pragma once

#include "GLM/glm.hpp"

#include "Component.h"

class CameraComponent : public Component
{
	CameraComponent(unsigned int _gameObject = 0, glm::vec3 _up = glm::vec3(0), bool _mainCamera = false) :
		Component(_gameObject), m_Up(_up), m_MainCamera(_mainCamera) {}

	glm::vec3 m_Up;

	float m_Pov = 90, m_NearPlane = 0.1f, m_FarPlane = 250.0f;

	bool m_MainCamera = false;
};