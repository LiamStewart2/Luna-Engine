#include "Camera.h"

Camera::Camera(glm::vec3 _position, glm::vec3 _direction, glm::vec3 _up)
{
    position = _position; direction = _direction; up = _up;
}

Camera::~Camera()
{
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(position, position + direction, up);
}
