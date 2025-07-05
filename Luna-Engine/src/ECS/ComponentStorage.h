#pragma once

template <typename T>
class ComponentStorage
{
public:
	std::unordered_map<unsigned int, T> components;

	template <typename... Args>
	T* AddComponent(unsigned int objectID, Args**... args);
	T* GetComponent(unsigned int objectID);

	void RemoveComponent(unsigned int objectID);
};

