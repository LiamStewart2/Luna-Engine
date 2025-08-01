#include "Camera.h"

glm::mat4 PerspectiveCamera::GetProjection()
{
    return glm::perspective(glm::radians(m_Pov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, m_NearPlane, m_FarPlane);
}

glm::mat4 PerspectiveCamera::GetView(Transform* transform)
{
    return glm::lookAt(transform->position, transform->position + transform.forward, transform.up);
}