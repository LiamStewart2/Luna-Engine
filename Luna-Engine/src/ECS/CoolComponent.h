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
		if (!gameObject) {
			std::cerr << "ERROR: CoolComponent has null gameObject!\n";
			return;
		}

		auto transform = gameObject->GetComponent<Transform>();
		if (!transform) {
			std::cerr << "ERROR: CoolComponent missing Transform!\n";
			return;
		}
		else
		{
			gameObject->GetComponent<Transform>()->rotation.z += (glfwGetTime() - lastTime) * 360;
			lastTime = glfwGetTime();
		}
	}
private:
	float lastTime = 0;
};