#include "Camera.h"

Camera::Camera()
{
    m_NearPlane = 0.1f;
    m_FarPlane = 100.0f;
    m_Pov = 90.0f;
}

glm::mat4 PerspectiveCamera::GetProjection()
{
    return glm::perspective(glm::radians(m_Pov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, m_NearPlane, m_FarPlane);
}


glm::mat4 PerspectiveCamera::GetView(Transform* transform)
{
    return glm::lookAt(transform->position, transform->position + transform->Forward(), transform->Up());
}