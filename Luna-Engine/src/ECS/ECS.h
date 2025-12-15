#pragma once

#include <unordered_map>
#include <typeindex>
#include <memory>

#include "ComponentStorage.h"

#include "NameComponent.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "LightComponent.h"
#include "CameraComponent.h"
#include "ScriptComponent.h"
#include "PhysicsComponent.h"

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

	// checks if gameObject contains component of type T
	template <typename T>
	bool HasComponent(unsigned int gameObject)
	{
		return GetOrCreatePool<T>().HasComponent(gameObject);
	}

	// removes all components from gameObject
	void RemoveAllComponents(unsigned int gameObject)
	{
		if (HasComponent<NameComponent>(gameObject))
			RemoveComponent<NameComponent>(gameObject);

		if (HasComponent<Transform>(gameObject))
			RemoveComponent<Transform>(gameObject);

		if (HasComponent<MeshComponent>(gameObject))
			RemoveComponent<MeshComponent>(gameObject);

		if (HasComponent<LightComponent>(gameObject))
			RemoveComponent<LightComponent>(gameObject);

		if (HasComponent<CameraComponent>(gameObject))
			RemoveComponent<CameraComponent>(gameObject);

		if (HasComponent<ScriptComponent>(gameObject))
			RemoveComponent<ScriptComponent>(gameObject);

		if (HasComponent<PhysicsComponent>(gameObject))
			RemoveComponent<PhysicsComponent>(gameObject);
	}

	// removes a component of type T from gameObject
	template <typename T>
	void RemoveComponent(unsigned int gameObject)
	{
		GetOrCreatePool<T>().RemoveComponent(gameObject);
	}

	// returns all components of type T
	template <typename T>
	std::unordered_map<unsigned int, T>& GetAllComponentsOfType()
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

