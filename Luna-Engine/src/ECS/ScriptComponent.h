#pragma once

#include <string>

#include "Component.h"

class Script;

struct ScriptComponent : public Component
{
	ScriptComponent(unsigned int _gameObject = 0, std::shared_ptr<Script> script = nullptr) :
		Component(_gameObject), m_Script(script)
	{
	}

	std::shared_ptr<Script> m_Script;
};