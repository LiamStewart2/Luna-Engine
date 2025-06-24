#pragma once

#include <vector>

#include "Component.h"

class GameObject
{
public:
	GameObject();

	void AddComponent(Component* component);
	
private:
	std::vector<Component*> components;
};

