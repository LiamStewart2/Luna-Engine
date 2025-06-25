#pragma once

#include <iostream>


#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Component.h"

class CoolComponent : public Component
{
public:
	CoolComponent(GameObject* _gameObject) : Component(_gameObject) {}

	void Update() override
	{
		gameObject->GetComponent<Transform>()->position.x = sin(glfwGetTime());
		std::cout << gameObject->GetComponent<Transform>()->position.x << std::endl;
	}

	int coolNumber = 4;
};