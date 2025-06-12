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
    glm::dvec2 currentMousePosition = glm::vec2(0, 0);
    glfwGetCursorPos(window, &currentMousePosition.x, &currentMousePosition.y);

    if(glm::dvec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2) == currentMousePosition)
        return;

    glm::dvec2 mouseMovement = currentMousePosition - glm::dvec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

    rotation.y -= mouseMovement.x * sensitivity;
    rotation.x += mouseMovement.y * sensitivity;

    CalculateDirection();

    glfwSetCursorPos(window, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
}

void Camera::CalculateDirection()
{
    forward.x = glm::sin(glm::radians(rotation.y));
    forward.y = glm::cos(glm::radians(rotation.x + 90));
    forward.z = glm::cos(glm::radians(rotation.y));
}
