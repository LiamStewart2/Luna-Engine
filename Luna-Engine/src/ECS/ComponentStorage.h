#pragma once

template <typename T>
class ComponentStorage
{
public:
	std::unordered_map<unsigned int, T> components;

	template <typename... Args>
	T* AddComponent(unsigned int objectID, Args&&... args)
	{
		T component(std::forward<Args>(args)...);
		return components[objectID] = std::move(component);
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
		for(auto it = components._Unchecked_begin(); it != components.end(); it++)
			results.push_back(&its->second);
		return result;
	}

	void RemoveComponent(unsigned int objectID)
	{
		components.erase(objectID);
	}
};