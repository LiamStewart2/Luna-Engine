#include "ScriptManager.h"

ScriptManager::ScriptManager()
{
	m_LuaState.open_libraries(sol::lib::base);
}

ScriptManager::~ScriptManager()
{
}

void ScriptManager::OnSceneLoad(const char* scriptFilepath)
{

}

void ScriptManager::OnSceneUpdate()
{
}

void ScriptManager::OnSceneClose()
{
}
