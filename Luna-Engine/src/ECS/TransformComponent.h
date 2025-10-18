#pragma once

#include <GLM/glm.hpp>
#include "GLM/gtx/matrix_decompose.hpp"

#include "Component.h"

struct Transform : public Component
{
	Transform(unsigned int _gameObject = 0, glm::vec3 _position = glm::vec3(0), glm::quat _rotation = glm::quat(1, 0, 0, 0), glm::vec3 _scale = glm::vec3(1)) : 
		Component(_gameObject), position(_position), rotation(_rotation), scale(_scale), transformMatrix(glm::mat4(1)), parentMatrix(glm::mat4(1)) {}
	
	glm::vec3 position, scale;

	glm::quat rotation;
	glm::mat4 transformMatrix, parentMatrix;

	void SetComponentsFromMatrix(const glm::mat4& worldMatrix)
	{
		//transformMatrix = worldMatrix;

		glm::mat4 localMatrix = glm::inverse(parentMatrix) * worldMatrix;

		glm::vec3 skew; glm::vec4 perspective;

		glm::decompose(localMatrix, scale, rotation, position, skew, perspective);
	}

	glm::vec3 Forward()
	{
		return rotation * glm::vec3(0, 0, -1);
	}
	glm::vec3 Up()
	{
		return glm::vec3(0, 1, 0);
	}

	// Returns the rotation in radians
	glm::vec3 GetEulerRotation()
	{
		return glm::eulerAngles(rotation);
	}

	// sets the rotation in radians
	void SetEulerRotation(glm::vec3 rot)
	{
		rotation = glm::quat(rot);
	}
};

template <class T>
struct ObjectTransformPairing
{
	T* object;
	Transform* objectTransform;
};