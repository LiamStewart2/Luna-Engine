#pragma once

#include <string>

#include "Component.h"
#include "../Scripting/script.h"

struct ScriptComponent : public Component
{
	ScriptComponent(unsigned int _gameObject = 0, std::shared_ptr<Script> script) :
		Component(_gameObject), m_Script(script)
	{
	}

	std::shared_ptr<Script> m_Script;
};