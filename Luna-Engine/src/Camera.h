#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera(glm::vec3 _position, glm::vec3 _direction, glm::vec3 _up);
	~Camera();

	glm::mat4 GetViewMatrix();

	glm::vec3 position, direction, up;
};