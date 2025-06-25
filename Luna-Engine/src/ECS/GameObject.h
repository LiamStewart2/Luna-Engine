#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include "Component.h"
#include "Transform.h"

class GameObject
{
public:
	GameObject();
	GameObject(glm::vec3 _position);

	std::string name;

	void Update();
	void OnRender(Renderer* renderer);

	template <typename T, typename... Args>
	std::shared_ptr<T> AddComponent(Args&&... args);
	
	template <typename T>
	std::shared_ptr<T> GetComponent();
private:
	std::vector<std::shared_ptr<Component>> components;
};

template <typename T, typename... Args>
std::shared_ptr<T> GameObject::AddComponent(Args&&... args)
{
	// Throw an error if the component class provided isnt derived from the Component Class
	static_assert(std::is_base_of<Component, T>::value, "Added component must be derived from Component Class");

	auto component = std::make_shared<T>(this, std::forward<Args>(args)...);
	components.push_back(component);
	return component;
}

template <typename T>
std::shared_ptr<T> GameObject::GetComponent()
{
	static_assert(std::is_base_of<Component, T>::value, "Searched component must be derived from Component Class");

	for (auto& component : components)
	{
		if(auto casted = std::dynamic_pointer_cast<T>(component))
			return casted;
	}
	return nullptr;
}