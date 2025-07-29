#include "Camera.h"

Camera::Camera(glm::vec3 _position, glm::vec3 _rotation)
{
    position = _position; rotation = _rotation;
    up = glm::vec3(0, 1, 0);
    CalculateDirection();
}

Camera::~Camera()
{
}

glm::mat4 Camera::GetProjection()
{
    return glm::perspective(glm::radians(pov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, nearPlane, farPlane);
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(position, position + forward, up);
}


void Camera::HandleInput(Window* window)
{
    
}



void Camera::CalculateDirection()
{
    forward.x =  glm::cos(glm::radians(rotation.y)) * glm::sin(glm::radians(rotation.z));
    forward.y = -glm::sin(glm::radians(rotation.y));
    forward.z =  glm::cos(glm::radians(rotation.y)) * glm::cos(glm::radians(rotation.z));
}
