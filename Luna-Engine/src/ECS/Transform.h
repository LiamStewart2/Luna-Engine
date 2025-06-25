#pragma once

#include "GLM/glm.hpp"

#include "Component.h"

class Transform : public Component
{
public:
	Transform(GameObject* _gameObject) : Component(_gameObject) {}
	Transform(GameObject* _gameObject, glm::vec3 _position) : Component(_gameObject), position(_position) {}
	Transform(GameObject* _gameObject, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale) : Component(_gameObject), position(_position), rotation(_rotation), scale(_scale) {}

	glm::vec3 position	= glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 scale		= glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 rotation	= glm::vec3(0.0f, 0.0f, 0.0f);
};