#include "Camera.h"

Camera::Camera(glm::vec3 _position, glm::vec3 _direction, glm::vec3 _up)
{
    position = _position; direction = _direction; up = _up;
    SetYawPitchRollWithDirection(_direction);
    mousePosition = {0, 0};
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
    HandleKeyboard(window);
    HandleMouse(window);
}

void Camera::HandleKeyboard(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        glm::vec3 movement = glm::normalize(glm::cross(direction, up));
        movement *= 0.03;
        position -= movement;
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        glm::vec3 movement = glm::normalize(glm::cross(direction, up));
        movement *= 0.03;
        position += movement;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        glm::vec3 movement = direction;
        movement *= 0.03;
        position += movement;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        glm::vec3 movement = direction;
        movement *= 0.03;
        position -= movement;
    }
}

void Camera::HandleMouse(GLFWwindow* window)
{

}

void Camera::SetYawPitchRollWithDirection(glm::vec3 _direction)
{
    _direction = glm::normalize(_direction);
    yaw = glm::degrees(atan2(_direction.z, _direction.x));
    pitch = glm::degrees(asin(_direction.y));
}

void Camera::CalculateDirection()
{
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
}
