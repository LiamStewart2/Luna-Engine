#pragma once

#include <memory>

#include "GameObject.h"

class Component
{
public:
	Component(GameObject) {}

protected:
	GameObject* gameObject = nullptr;

private:
	void Print() {}
};

