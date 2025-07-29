#pragma once

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Window.h"
#include "Time.h"
#include "Globals.h"

class Camera
{
public:
	Camera(glm::vec3 _position, glm::vec3 _rotation);
	~Camera();

	glm::mat4 GetProjection();
	glm::mat4 GetViewMatrix();
	void HandleInput(Window* window);

	glm::vec3 GetPosition(){return position;}

	float nearPlane = 0.1f, farPlane = 250.0f;
	float pov = 90.0f;

private:

	glm::vec3 position, forward, up;
	glm::vec3 rotation;

	void CalculateDirection();
};