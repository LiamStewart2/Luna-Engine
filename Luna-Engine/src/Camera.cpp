#include "Camera.h"

Camera::Camera(glm::vec3 _position, glm::vec3 _rotation)
{
    position = _position; rotation = _rotation;
    CalculateDirection();

    mousePosition = {0, 0};
}

Camera::~Camera()
{
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(position, position + forward, up);
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
        glm::vec3 movement = glm::normalize(glm::cross(forward, up));
        movement *= 0.03;
        position -= movement;
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        glm::vec3 movement = glm::normalize(glm::cross(forward, up));
        movement *= 0.03;
        position += movement;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        glm::vec3 movement = forward;
        movement *= 0.03;
        position += movement;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        glm::vec3 movement = forward;
        movement *= 0.03;
        position -= movement;
    }
}

void Camera::HandleMouse(GLFWwindow* window)
{

}

void Camera::CalculateDirection()
{
    
}
