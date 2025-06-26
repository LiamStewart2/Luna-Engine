#include "GameObject.h"

GameObject::GameObject()
{
	components = std::vector<std::shared_ptr<Component>>();
	AddComponent<Transform>();
}

GameObject::GameObject(glm::vec3 _position)
{
	components = std::vector<std::shared_ptr<Component>>();
	AddComponent<Transform>(_position);
}

GameObject::GameObject(glm::vec3 _position, glm::vec3 _scale, glm::vec3 _rotation)
{
	components = std::vector<std::shared_ptr<Component>>();
	AddComponent<Transform>(_position, _scale, _rotation);
}

void GameObject::OnStart()
{
	for (size_t i = 0; i < components.size(); i++)
		components[i].get()->OnStart();
}

void GameObject::Update()
{
	for (size_t i = 0; i < components.size(); i++)
		components[i].get()->Update();
}

void GameObject::OnRender(Renderer* renderer)
{
	for (size_t i = 0; i < components.size(); i++)
		components[i].get()->OnRender(renderer);
}