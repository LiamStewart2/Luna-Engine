#pragma once

#include <vector>
#include <string>
#include <memory>

#include "Component.h"

class GameObject
{
public:
	GameObject();

	std::string name;

	template <typename T>
	std::weak_ptr<T> AddComponent();
	
	template <typename T>
	std::weak_ptr<T> GetComponent();
private:
	std::vector<std::shared_ptr<Component*>> components;
};

