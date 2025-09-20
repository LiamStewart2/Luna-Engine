#pragma once

#define SOL_ALL_SAFETIES_ON 1
#include <SOL/sol.hpp>

#include <fstream>

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

