#pragma once

#include "GLM/glm.hpp"
#include "GLM/gtx/quaternion.hpp"

#include "Stack.h"
#include "SceneGraph.h"
#include "../ECS/ECS.h"

class TransformationManager
{
public:
	TransformationManager() {}
	~TransformationManager() {}

	void UpdateTransformationMatricies(SceneGraph* sceneGraph, EntityComponentSystem* ECS);

private:
	const int Mat4StackSize = 15;
	Stack<glm::mat4> m_Stack = Stack<glm::mat4>(Mat4StackSize);
	void UpdateTransform(SceneGraphNode* node, std::unordered_map<unsigned int, Transform*>* transformComponents, glm::mat4 parentMatrix = glm::mat4(1));
};