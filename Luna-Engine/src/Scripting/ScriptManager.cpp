#include "ScriptManager.h"

ScriptManager::ScriptManager()
{

}

ScriptManager::~ScriptManager()
{
}

void ScriptManager::OnSceneLoad(EntityComponentSystem* ECS)
{
	std::unordered_map<unsigned int, ScriptComponent*> scriptComponents = ECS->GetAllComponentsOfType<ScriptComponent>();
	for (auto [id, component] : scriptComponents)
	{
		
	}
}

void ScriptManager::OnSceneUpdate(EntityComponentSystem* ECS)
{
	std::unordered_map<unsigned int, ScriptComponent*> scriptComponents = ECS->GetAllComponentsOfType<ScriptComponent>();
	for (auto [id, component] : scriptComponents)
	{
		component->m_Script->Execute(id);
	}
}

void ScriptManager::OnSceneClose()
{
}
