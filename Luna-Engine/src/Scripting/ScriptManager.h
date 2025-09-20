#pragma once

#include "SOL/sol.hpp"

class ScriptManager
{
public:
	ScriptManager();
	~ScriptManager();

	void OnSceneLoad(const char* scriptFilepath);
	void OnSceneUpdate();
	void OnSceneClose();

private:
	sol::state m_LuaState;
};

