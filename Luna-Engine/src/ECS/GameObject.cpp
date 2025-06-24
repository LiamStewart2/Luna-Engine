#include "GameObject.h"

GameObject::GameObject()
{
	components = std::vector<std::shared_ptr<Component>>();
}

void GameObject::Update()
{
	for (size_t i = 0; i < components.size(); i++)
		components[i].get()->Update();
}