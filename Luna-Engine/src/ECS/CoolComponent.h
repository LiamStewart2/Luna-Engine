#pragma once

#include <iostream>

#include "Component.h"

class CoolComponent : public Component
{
public:
	CoolComponent(GameObject* _gameObject) : Component(_gameObject) {}

	void Update() override
	{
		gameObject->GetComponent<Transform>()->position.x += 0.1f;
		std::cout << gameObject->GetComponent<Transform>()->position.x << std::endl;
	}

	int coolNumber = 4;
};