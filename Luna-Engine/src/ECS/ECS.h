#pragma once

#include <unordered_map>
#include <typeindex>
#include <memory>

#include "ComponentStorage.h"
#include "GameObject.h"

class EntityComponentSystem
{
public:
	EntityComponentSystem();
	~EntityComponentSystem();

	// returns the object component of type T
	template <typename T>
	T* GetObjectComponent(GameObject gameObject);

	// returns all components of type T
	template <typename T>
	std::vector<T*> GetAllComponentsOfType();

private:
	std::unordered_map<std::type_index, std::shared_ptr<void>> componentPools;

	template <typename T>
	ComponentStorage<T>* GetOrCreatePool();
};

