#include "TransformationManager.h"

void TransformationManager::UpdateTransformationMatricies(SceneGraph* sceneGraph, EntityComponentSystem* ECS)
{
	m_Stack.ResetStack(); m_Stack.Push(glm::mat4(1));
	std::unordered_map<unsigned int, Transform*> transformComponents = ECS->GetAllComponentsOfType<Transform>();

	UpdateTransform(sceneGraph, &transformComponents);
}


void TransformationManager::UpdateTransform(SceneGraphNode* node, std::unordered_map<unsigned int, Transform*>* transformComponents)
{
	glm::mat4 matrix = m_Stack.Peek();

	matrix = glm::translate(matrix, transformComponents->at(node->GetGameObject())->position);

	matrix = glm::rotate(matrix, transformComponents->at(node->GetGameObject())->rotation.x, {1, 0, 0});
	matrix = glm::rotate(matrix, transformComponents->at(node->GetGameObject())->rotation.y, { 0, 1, 0 });
	matrix = glm::rotate(matrix, transformComponents->at(node->GetGameObject())->rotation.z, { 0, 0, 1 });

	matrix = glm::scale(matrix, transformComponents->at(node->GetGameObject())->scale);

	transformComponents->at(node->GetGameObject())->transformMatrix = matrix;

	m_Stack.Push(matrix);

	for (SceneGraphNode* n : *node->getNodes())
		UpdateTransform(n, transformComponents);

	m_Stack.Pop();
}