#pragma once

#include <SOL/sol.hpp>
#include "../ECS/ECS.h"

class Script
{
public:
	Script(std::string filepath);
	~Script();

	void Compile(std::string filepath = "");

	void Execute();

	void Cool();

	std::string GetFilepath() {return m_Filepath;}
	EntityComponentSystem* m_ECS = nullptr;
private:

	sol::state m_Lua;
	sol::load_result m_CompiledScript;

	sol::protected_function m_Update;
	sol::protected_function m_Start;

	std::string m_Filepath;
};

