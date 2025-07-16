#pragma once

#include <unordered_map>
#include <iostream>

#include "Component.h"

template <typename T>
class ComponentStorage
{
public:
	std::unordered_map<unsigned int, T> components;

	template <typename... Args>
	T* AddComponent(unsigned int objectID, Args&&... args)
	{
		auto argsTuple = std::forward_as_tuple(args...); 
		T component(objectID, std::forward<Args>(args)...);
		components[objectID] = std::move(component);
		return &components[objectID];
	}
	T* GetComponent(unsigned int objectID)
	{
		auto it = components.find(objectID);
		if(it != components.end())
			return &it->second;
		return nullptr;
	}

	std::unordered_map<unsigned int, T*> GetComponents()
	{
		std::unordered_map<unsigned int, T*> result;
		for (auto& [id, component] : components)
			result[id] = &component;
		return result;
	}

	void RemoveComponent(unsigned int objectID)
	{
		components.erase(objectID);
	}

private:

};