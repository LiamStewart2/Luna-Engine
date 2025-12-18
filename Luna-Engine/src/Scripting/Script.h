#pragma once

#include <SOL/sol.hpp>
#include <filesystem>
#include "../ECS/ECS.h"

class Script
{
public:
	Script(std::string filepath);
	~Script();

	void Compile(std::string filepath = "");
	void BindFunctions();

	void Execute(unsigned int gameobject);

	bool NeedsCompiling();

	std::string GetFilepath() {return m_Filepath;}
	EntityComponentSystem* m_ECS = nullptr;

	unsigned int GetGameObject() { return m_GameObject; }
private:
	unsigned int m_GameObject = 0;
	std::filesystem::file_time_type m_LastCompilationTime;
	sol::state m_Lua;
	sol::load_result m_CompiledScript;

	sol::protected_function m_Update;
	sol::protected_function m_Start;

	std::string m_Filepath;
};

