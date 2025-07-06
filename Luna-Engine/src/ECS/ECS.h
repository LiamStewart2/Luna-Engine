#pragma once

#include <unordered_map>
#include <typeindex>
#include <memory>

#include "ComponentStorage.h"

#include "TransformComponent.h"
#include "MeshComponent.h"

class EntityComponentSystem
{
public:
	// returns the object component of type T
	template <typename T>
	T* GetObjectComponent(unsigned int gameObject)
	{
		return GetOrCreatePool<T>().GetComponent(gameObject);
	}

	// adds a component to gameObject of type T. returns a reference to the added component
	template <typename T, typename... Args>
	T* AddComponent(unsigned int gameObject, Args&&... args)
	{
		return GetOrCreatePool<T>().AddComponent(gameObject, std::forward<Args>(args)...);
	}

	// removes a component of type T from gameObject
	template <typename T>
	void RemoveComponent(unsigned int gameObject)
	{
		GetOrCreatePool<T>().RemoveComponent(gameObject);
	}

	// returns all components of type T
	template <typename T>
	std::vector<T*> GetAllComponentsOfType()
	{
		return GetOrCreatePool<T>().GetComponents();
	}

private:
	std::unordered_map<std::type_index, std::shared_ptr<void>> componentPools;

	template <typename T>
	ComponentStorage<T>& GetOrCreatePool()
	{
		static_assert(std::is_base_of<Component, T>::value, "Component must be derived from Component Class!");
		std::type_index type = typeid(T);
		if (componentPools.find(type) == componentPools.end())
			componentPools[type] = std::make_shared<ComponentStorage<T>>();
		return *std::static_pointer_cast<ComponentStorage<T>>(componentPools[type]);
	}
};

