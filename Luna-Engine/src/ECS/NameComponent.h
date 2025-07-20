#pragma once

#include "Component.h"

#include <string>

struct NameComponent : public Component
{
	NameComponent(unsigned int _gameObject = 0, std::string _name = "Game Object") : Component(_gameObject), m_Name(_name) {}

	std::string m_Name = "Game Object";
};