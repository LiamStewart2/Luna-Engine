// Luna Engine - Transformation Manager

#pragma once

#include "GLM/glm.hpp"
#include "GLM/gtx/quaternion.hpp"

#include "Stack.h"
#include "SceneGraph.h"
#include "../ECS/ECS.h"

/*
	Updates all transform components in the ECS using their hierachical relations to each other stored in a scene graph
*/
class TransformationManager
{
public:
	TransformationManager() {}
	~TransformationManager() {}

	// void UpdateTransformationMatricies() - iterates through all transform components in the ECS to update their world matrix
	// SceneGraph* sceneGraph - a reference to the root node of the scenes scene graph
	// EntityComponentSystem* ECS - a reference to the ECS of the scene
	void UpdateTransformationMatricies(SceneGraph* sceneGraph, EntityComponentSystem* ECS);

private:
	const int Mat4StackSize = 15;
	Stack<glm::mat4> m_Stack = Stack<glm::mat4>(Mat4StackSize);

	// void UpdateTransform() - the recursive function called for each scene graph node in the tree to set the world matrix
	// SceneGraphNode* node - the current node being calculated
	// std::unordered_map<unsigned int, Transform*>* transformComponents - a reference to all of the scenes transform components
	// glm::mat4 parentMatrix - the matrix of the parent transform of the current node
	void UpdateTransform(SceneGraphNode* node, std::unordered_map<unsigned int, Transform>& transformComponents, glm::mat4 parentMatrix = glm::mat4(1));
};