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
	void OnSceneUpdate(std::unordered_map<unsigned int, ScriptComponent>& scriptComponents);
	void OnSceneClose();

private:
	sol::state m_LuaState;
};

