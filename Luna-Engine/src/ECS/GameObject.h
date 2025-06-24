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

	void Update();

	template <typename T>
	std::weak_ptr<T> AddComponent();
	
	template <typename T>
	std::weak_ptr<T> GetComponent();
private:
	std::vector<std::shared_ptr<Component>> components;
};

template <typename T>
std::weak_ptr<T> GameObject::AddComponent()
{
	// Throw an error if the component class provided isnt derived from the Component Class
	static_assert(std::is_base_of<Component, T>::value, "Added component must be derived from Component Class");

	components.push_back(std::make_shared<T>(this));
	return std::static_pointer_cast<T>(components[components.size() - 1]);
}

template <typename T>
std::weak_ptr<T> GameObject::GetComponent()
{
	static_assert(std::is_base_of<Component, T>::value, "Searched component must be derived from Component Class");

	for (size_t i = 0; i <= components.size(); i++)
	{
		if (typeid(T*) == typeid(components[i]))
			return components[i];
	}
	return nullptr;
}