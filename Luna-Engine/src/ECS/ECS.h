#pragma once

#include <unordered_map>
#include <typeindex>
#include <memory>

#include "ComponentStorage.h"
#include "GameObject.h"

class EntityComponentSystem
{
public:
	// returns the object component of type T
	template <typename T>
	T* GetObjectComponent(GameObject gameObject)
	{
		return GetOrCreatePool<T>()->GetComponent(gameObject.ID);
	}

	// adds a component to gameObject of type T. returns a reference to the added component
	template <typename T, typename... Args>
	T* AddComponent(GameObject gameObject, Args&&... args)
	{
		return GetOrCreatePool<T>()->AddComponent(gameObject.ID, std::forward<Args>(args)...);
	}

	// removes a component of type T from gameObject
	template <typename T>
	void RemoveComponent(GameObject gameObject)
	{
		GetOrCreatePool<T>()->RemoveComponent(gameObject);
	}

	// returns all components of type T
	template <typename T>
	std::vector<T*> GetAllComponentsOfType()
	{
		return GetOrCreatePool<T>()->GetComponents();
	}

private:
	std::unordered_map<std::type_index, std::shared_ptr<void>> componentPools;

	template <typename T>
	ComponentStorage<T>* GetOrCreatePool()
	{
		std::type_index type = typeid(T);
		if (componentPools.find(type) == componentPools.end())
			componentPools[type] = std::make_shared<ComponentStorage<T>>();
		return *std::static_pointer_cast<ComponentStorage<T>>(componentPools[type]);
	}
};

