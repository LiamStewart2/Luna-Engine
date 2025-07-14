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
    HandleKeyboard(window);
    HandleMouse(window);
}

void Camera::HandleKeyboard(Window* window)
{
    if (window->GetKey(GLFW_KEY_A) == GLFW_PRESS)
    {
        glm::vec3 movement = glm::normalize(glm::cross(forward, up));
        movement *= 0.03;
        position -= movement;
    }
    else if (window->GetKey(GLFW_KEY_D) == GLFW_PRESS)
    {
        glm::vec3 movement = glm::normalize(glm::cross(forward, up));
        movement *= 0.03;
        position += movement;
    }

    if (window->GetKey(GLFW_KEY_W) == GLFW_PRESS)
    {
        glm::vec3 movement = forward;
        movement *= 0.03;
        position += movement;
    }
    else if (window->GetKey(GLFW_KEY_S) == GLFW_PRESS)
    {
        glm::vec3 movement = forward;
        movement *= 0.03;
        position -= movement;
    }

    if (window->GetKey(GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        glm::vec3 movement = up;
        movement *= 0.03;
        position += movement;
    }
}

void Camera::HandleMouse(Window* window)
{
    glm::dvec2 currentMousePosition = glm::vec2(0, 0);
    window->GetCursorPosition(& currentMousePosition.x, & currentMousePosition.y);

    if(glm::dvec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2) == currentMousePosition)
        return;

    glm::dvec2 mouseMovement = currentMousePosition - glm::dvec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

    rotation.z -= mouseMovement.x * sensitivity;
    rotation.y += mouseMovement.y * sensitivity;

    CalculateDirection();

    window->SetCursorPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
}

void Camera::CalculateDirection()
{
    forward.x =  glm::cos(glm::radians(rotation.y)) * glm::sin(glm::radians(rotation.z));
    forward.y = -glm::sin(glm::radians(rotation.y));
    forward.z =  glm::cos(glm::radians(rotation.y)) * glm::cos(glm::radians(rotation.z));
}
