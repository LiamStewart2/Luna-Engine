#pragma once

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Window.h"
#include "Time.h"
#include "ECS/TransformComponent.h"
#include "Globals.h"

class Camera
{
public:
	Camera() {}
	~Camera() {}

	virtual glm::mat4 GetProjection() = 0;
	virtual glm::mat4 GetView(Transform* transform) = 0;

	const float m_NearPlane = 0.1f, m_FarPlane = 250.0f;
	const float m_Pov = 90.0f;
};


class PerspectiveCamera : public Camera
{
public:
	glm::mat4 GetProjection() override;
	glm::mat4 GetView(Transform* transform) override;
};