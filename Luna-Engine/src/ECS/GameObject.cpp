#include "GameObject.h"

GameObject::GameObject()
{

}

template <typename T>
std::weak_ptr<T> GameObject::AddComponent()
{
	// Throw an error if the component class provided isnt derived from the Component Class
	static_asset(std::is_base_of<Component, T>::value, "Added component must be derived from Component Class - " + name.c_str());
	
	components.push_back(std::make_shared<T>(GameObject));
	return components[components.size() - 1];
}

template <typename T>
std::weak_ptr<T> GameObject::GetComponent()
{
	for (size_t i = 0; i <= components.size(); i++)
	{
		if (typeid(T*) == typeid(components[i]))
			return components[i];
	}
	return nullptr;
}