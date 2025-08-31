#pragma once

#include "Camera.h"
#include "LunaWindow.h"

#include <iostream>

class EditorCamera : Camera
{
public:
	EditorCamera(glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 rotation = glm::vec3(0, 0, 0))
		: m_Position(position), m_Rotation(rotation) {}

	glm::mat4 GetProjection() override;
	glm::mat4 GetView(Transform* transform = nullptr) override;

	void Update();

	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
private:
	void HandleMovement();
	void HandleRotation();

	glm::vec3 Forward();

	float m_MovementSpeed = 0.2f;
	float m_Sensitivity = 1.0f;
	bool m_FirstMousePressFrame = true;
	glm::dvec2 m_LastMousePosition = glm::dvec2(0);
};