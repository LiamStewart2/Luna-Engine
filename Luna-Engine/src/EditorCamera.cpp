#include "EditorCamera.h"

bool EditorCamera::sceneWindowFocused = false;
bool EditorCamera::sceneWindowHovered = false;

glm::mat4 EditorCamera::GetProjection()
{
    return glm::perspective(glm::radians(m_Pov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, m_NearPlane, m_FarPlane);
}

glm::mat4 EditorCamera::GetView(Transform* transform)
{
    return glm::lookAt(m_Position, m_Position + Forward(), glm::vec3(0, 1, 0));
}

void EditorCamera::Update()
{
    HandleMovement();

    ImGuiIO& io = ImGui::GetIO();

    if (EditorCamera::sceneWindowFocused && EditorCamera::sceneWindowHovered &&
        LunaWindow::m_FocusedWindow->GetMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange; // stop ImGui overriding
        HandleRotation();
    }
    else
    {
        io.ConfigFlags &= ~ImGuiConfigFlags_NoMouseCursorChange; // restore ImGui behaviour
        if (m_FirstMousePressFrame)
        {
            m_FirstMousePressFrame = false;
            LunaWindow::m_FocusedWindow->SetCursorPosition(m_LastMousePosition.x, m_LastMousePosition.y);
            LunaWindow::m_FocusedWindow->SetInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}

void EditorCamera::HandleMovement()
{
    LunaWindow* window = LunaWindow::m_FocusedWindow;

    if (window->GetKey(GLFW_KEY_W))
        m_Position += Forward() * m_MovementSpeed;
    if(window->GetKey(GLFW_KEY_S))
        m_Position -= Forward() * m_MovementSpeed;
    if(window->GetKey(GLFW_KEY_D))
        m_Position += normalize(glm::cross(Forward(), glm::vec3(0, 1, 0))) * m_MovementSpeed;
    if (window->GetKey(GLFW_KEY_A))
        m_Position -= normalize(glm::cross(Forward(), glm::vec3(0, 1, 0))) * m_MovementSpeed;
}

void EditorCamera::HandleRotation()
{
    LunaWindow* window = LunaWindow::m_FocusedWindow;

    if (m_FirstMousePressFrame == false)
    {
        m_FirstMousePressFrame = true;
        window->SetInputMode(GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        window->GetCursorPosition(&m_LastMousePosition.x, &m_LastMousePosition.y);
        window->SetCursorPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
        return;
    }

    glm::dvec2 currentMousePosition = glm::vec2(0, 0);
    window->GetCursorPosition(&currentMousePosition.x, &currentMousePosition.y);

    if (glm::dvec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2) == currentMousePosition)
        return;

    glm::dvec2 mouseMovement = currentMousePosition - glm::dvec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

    m_Rotation.x += mouseMovement.x * m_Sensitivity;
    m_Rotation.y -= mouseMovement.y * m_Sensitivity;

    m_Rotation.y = glm::clamp(m_Rotation.y, -89.0f, 89.0f);

    window->SetCursorPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
}

glm::vec3 EditorCamera::Forward()
{
    float yaw = glm::radians(m_Rotation.x);
    float pitch = glm::radians(m_Rotation.y);

    glm::vec3 forward;
    forward.x = cos(pitch) * cos(yaw);
    forward.y = sin(pitch);
    forward.z = cos(pitch) * sin(yaw);

    return glm::normalize(forward);
}