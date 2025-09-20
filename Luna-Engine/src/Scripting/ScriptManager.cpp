#include "ScriptManager.h"

ScriptManager::ScriptManager()
{
	m_LuaState.open_libraries(sol::lib::base, sol::lib::math);

}

ScriptManager::~ScriptManager()
{
}

void ScriptManager::OnSceneLoad(const char* scriptFilepath)
{
	m_LuaState.script_file(scriptFilepath);
}

void ScriptManager::OnSceneUpdate()
{
}

void ScriptManager::OnSceneClose()
{
}
