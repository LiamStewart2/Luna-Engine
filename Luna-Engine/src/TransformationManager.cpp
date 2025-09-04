#include "TransformationManager.h"

void TransformationManager::UpdateTransformationMatricies(SceneGraph* sceneGraph, EntityComponentSystem* ECS)
{
	m_Stack.ResetStack(); m_Stack.Push(glm::mat4(1));
	std::unordered_map<unsigned int, Transform*> transformComponents = ECS->GetAllComponentsOfType<Transform>();

	UpdateTransform(sceneGraph, &transformComponents);
}


void TransformationManager::UpdateTransform(SceneGraphNode* node, std::unordered_map<unsigned int, Transform*>* transformComponents, glm::mat4 parentMatrix)
{
	/// Calculate world matrix for the transform component
	glm::mat4 matrix = m_Stack.Peek();

	Transform* transform = transformComponents->at(node->GetGameObject());

	matrix = glm::translate(matrix, transform->position);

	matrix *= glm::toMat4(transform->rotation);

	matrix = glm::scale(matrix, transformComponents->at(node->GetGameObject())->scale);

	transformComponents->at(node->GetGameObject())->transformMatrix = matrix;
	transformComponents->at(node->GetGameObject())->parentMatrix = parentMatrix;

	m_Stack.Push(matrix);
	/// Update all child nodes matricies
	for (SceneGraphNode* n : *node->getNodes())
		UpdateTransform(n, transformComponents, matrix);

	m_Stack.Pop();
}