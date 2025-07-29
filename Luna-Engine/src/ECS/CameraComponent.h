#pragma once

#include "GLM/glm.hpp"

#include "Component.h"

struct CameraComponent : public Component
{
	CameraComponent(unsigned int _gameObject = 0, glm::vec3 _up = glm::vec3(0, 1, 0), glm::vec3 _forward = glm::vec3(0, 0, -1), bool _mainCamera = false) :
		Component(_gameObject), m_Up(_up), m_Forward(_forward), m_MainCamera(_mainCamera) {}

	glm::vec3 m_Up, m_Forward;

	float m_Pov = 90, m_NearPlane = 0.1f, m_FarPlane = 250.0f;

	bool m_MainCamera = false;
};