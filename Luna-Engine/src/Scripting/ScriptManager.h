#pragma once

#include <SOL/sol.hpp>
#include <fstream>

#include "../ECS/ECS.h"

class ScriptManager
{
public:
	ScriptManager();
	~ScriptManager();
	void OnSceneLoad();
	void OnSceneUpdate();
	void OnSceneClose();

private:
	sol::state m_LuaState;
};

