#pragma once

#include "GLM/glm.hpp"

#include "Component.h"

class CameraComponent : public Component
{
	glm::vec3 up;
	float m_NearPlane = 0.1f, m_FarPlane = 250.0f;
	float pov = 90.0f;
};