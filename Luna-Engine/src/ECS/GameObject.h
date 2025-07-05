#pragma once

#include "ECS.h"

class GameObject 
{
public:
	GameObject(unsigned int  _ID);

	template <typename T, typename... Args>
	T* AddComponent(Args&&... args);

private:
	unsigned int ID;
};

template <typename T, typename... Args>
T* GameObject::AddComponent(Args&&... args)
{
	// Throw an error if the component class provided isnt derived from the Component Class
	static_assert(std::is_base_of<Component, T>::value, "Added component must be derived from Component Class");

	ECS->AddComponent<T>(ID, std::forward<Args>(args)...);
}