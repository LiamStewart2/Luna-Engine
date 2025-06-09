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


void Camera::HandleInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        position -= glm::normalize(glm::cross(position, up));
        direction -= glm::normalize(glm::cross(position, up));
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        position += glm::normalize(glm::cross(position, up));
        direction += glm::normalize(glm::cross(position, up));
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        position += direction;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        -position += direction;
    }
}