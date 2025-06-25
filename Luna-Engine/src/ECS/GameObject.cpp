#include "GameObject.h"

GameObject::GameObject()
{
	components = std::vector<std::shared_ptr<Component>>();
	
}

GameObject::GameObject(glm::vec3 _position)
{
	components = std::vector<std::shared_ptr<Component>>();
	AddComponent<Transform>(_position);
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