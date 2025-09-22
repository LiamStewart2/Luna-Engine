#pragma once

#include <SOL/sol.hpp>
#include <fstream>

#include "Script.h"
#include "../ECS/ECS.h"

class ScriptManager
{
public:
	ScriptManager();
	~ScriptManager();
	void OnSceneLoad(EntityComponentSystem* ECS);
	void OnSceneUpdate(EntityComponentSystem* ECS);
	void OnSceneClose();

private:
	sol::state m_LuaState;
};

