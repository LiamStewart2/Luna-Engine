//Luna Engine - Camera

#pragma once

#include <iostream>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../ECS/TransformComponent.h"
#include "Globals.h"

/*
Parent class for all camera types used for renderering
Provides variables and Functions for all camera types making sure the renderer has all data it requires

ToDo - rename m_EditorBackgroundColour to something more reasonable for the background colour
*/
class Camera
{
public:
	Camera();
	~Camera() {}

	// Required to be integrated by all child camera classes
	virtual glm::mat4 GetProjection() = 0;
	// Required to be integrated by all child camera classes
	// Transform* transform - a reference to the cameras transform, if using an object outside of the ECS create a temporary transform component on the stack
	virtual glm::mat4 GetView(Transform* transform) = 0;

	float m_NearPlane, m_FarPlane;
	float m_Pov;
};

// Luna Engine - Perspective Camera
/*
uses glm functions to build perspective matricies for common 3D renderering
*/
class PerspectiveCamera : public Camera
{
public:
	glm::mat4 GetProjection() override;
	glm::mat4 GetView(Transform* transform) override;
};