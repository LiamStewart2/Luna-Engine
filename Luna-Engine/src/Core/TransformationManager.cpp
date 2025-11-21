// Luna Engine - Transformation Manager

#include "TransformationManager.h"

// void UpdateTransformationMatricies() - iterates through all transform components in the ECS to update their world matrix
// SceneGraph* sceneGraph - a reference to the root node of the scenes scene graph
// EntityComponentSystem* ECS - a reference to the ECS of the scene
void TransformationManager::UpdateTransformationMatricies(SceneGraph* sceneGraph, EntityComponentSystem* ECS)
{
	m_Stack.ResetStack(); m_Stack.Push(glm::mat4(1));
	std::unordered_map<unsigned int, Transform>& transformComponents = ECS->GetAllComponentsOfType<Transform>();

	UpdateTransform(sceneGraph, transformComponents);
}

// void UpdateTransform() - the recursive function called for each scene graph node in the tree to set the world matrix
// SceneGraphNode* node - the current node being calculated
// std::unordered_map<unsigned int, Transform*>* transformComponents - a reference to all of the scenes transform components
// glm::mat4 parentMatrix - the matrix of the parent transform of the current node
void TransformationManager::UpdateTransform(SceneGraphNode* node, std::unordered_map<unsigned int, Transform>& transformComponents, glm::mat4 parentMatrix)
{
	/// Calculate world matrix for the transform component
	glm::mat4 matrix = m_Stack.Peek();

	Transform* transform = &transformComponents[node->GetGameObject()];

	matrix = glm::translate(matrix, transform->position);

	matrix *= glm::toMat4(transform->rotation);

	matrix = glm::scale(matrix, transform->scale);

	transform->transformMatrix = matrix;
	transform->parentMatrix = parentMatrix;

	m_Stack.Push(matrix);
	/// Update all child nodes matricies
	for (SceneGraphNode* n : *node->getNodes())
		UpdateTransform(n, transformComponents, matrix);

	m_Stack.Pop();
}