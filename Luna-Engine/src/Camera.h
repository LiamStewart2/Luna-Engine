#pragma once

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Time.h"

class Camera
{
public:
	Camera(glm::vec3 _position, glm::vec3 _direction, glm::vec3 _up);
	~Camera();

	glm::mat4 GetViewMatrix();
	void HandleInput(GLFWwindow* window);

	glm::vec3 position, direction, up;
};