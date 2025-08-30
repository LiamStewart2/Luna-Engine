#pragma once

#include "Camera.h"

class EditorCamera : Camera
{
public:
	glm::mat4 GetProjection() override;
	glm::mat4 GetView(Transform* transform = nullptr) override;

	void Update();
private:
	void HandleMovement();
	void HandleMouseMovement();

	glm::vec3 position;
	glm::vec3 rotation;
};