#pragma once

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Time.h"
#include "Globals.h"

class Camera
{
public:
	Camera(glm::vec3 _position, glm::vec3 _rotation);
	~Camera();

	glm::mat4 GetViewMatrix();
	void HandleInput(GLFWwindow* window);


private:
	void HandleKeyboard(GLFWwindow* window);
	void HandleMouse(GLFWwindow* window);

	glm::vec3 position, forward, up;
	glm::vec3 rotation;
	
	bool firstFrame = true;
	float sensitivity = 0.3f;

	void CalculateDirection();
};