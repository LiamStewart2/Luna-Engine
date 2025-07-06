#pragma once

#include <unordered_map>

#include "Component.h"

template <typename T>
class ComponentStorage
{
public:
	std::unordered_map<unsigned int, T> components;

	template <typename... Args>
	T* AddComponent(unsigned int objectID, Args&&... args)
	{
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

	std::vector<T*> GetComponents()
	{
		std::vector<T*> result;
		result.reserve(components.size());
		for(auto it = components.begin(); it != components.end(); it++)
			result.push_back(&it->second);
		return result;
	}

	void RemoveComponent(unsigned int objectID)
	{
		components.erase(objectID);
	}

private:

};