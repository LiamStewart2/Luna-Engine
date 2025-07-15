#pragma once

#include <GLM/glm.hpp>

#include "Component.h"

struct Transform : public Component
{
	Transform(unsigned int _gameObject = 0, glm::vec3 _position = glm::vec3(0), glm::vec3 _rotation = glm::vec3(0), glm::vec3 _scale = glm::vec3(1)) : 
		Component(_gameObject), position(_position), rotation(_rotation), scale(_scale), transformMatrix(glm::mat4(1)) {}
	
	glm::vec3 position, rotation, scale;
	glm::mat4 transformMatrix;
};