#pragma once

#include <iostream>

#include "Component.h"

class CoolComponent : public Component
{
public:
	CoolComponent(GameObject* _gameObject) : Component(_gameObject) {}

	void Update() override
	{
		std::cout << "cool component" << std::endl;
	}
};