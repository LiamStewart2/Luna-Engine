#include "EditorCamera.h"

glm::mat4 EditorCamera::GetProjection()
{
    return glm::perspective(glm::radians(m_Pov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, m_NearPlane, m_FarPlane);
}

glm::mat4 EditorCamera::GetView(Transform* transform)
{
    glm::vec3 forward = glm::vec3(sin(glm::radians(m_Rotation.y)), cos(glm::radians(m_Rotation.x + 90)), cos(glm::radians(m_Rotation.y)));
    return glm::lookAt(m_Position, m_Position + forward, glm::vec3(0, 1, 0));
}

void EditorCamera::Update()
{
    HandleMovement();
    HandleRotation();
}

void EditorCamera::HandleMovement()
{
    LunaWindow* window = LunaWindow::m_FocusedWindow;

    if (window->GetKey(GLFW_KEY_W))
    {
        std::cout << "Fortnite" << std::endl;
    }
}

void EditorCamera::HandleRotation()
{
    LunaWindow* window = LunaWindow::m_FocusedWindow;
}
