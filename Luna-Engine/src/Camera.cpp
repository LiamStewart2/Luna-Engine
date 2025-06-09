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
        glm::vec3 movement = glm::normalize(glm::cross(direction, up));
        movement *= 0.01;
        position -= movement;
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        glm::vec3 movement = glm::normalize(glm::cross(direction, up));
        movement *= 0.01;
        position += movement;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        glm::vec3 movement = direction;
        movement *= 0.01;
        position += movement;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        glm::vec3 movement = direction;
        movement *= 0.01;
        position -= movement;
    }

    std::cout << position.x << ", " << position.y << " << FRONT << " << direction.x << ", " << direction.y << ", " << direction.z << std::endl;
}