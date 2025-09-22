#pragma once

#include <SOL/sol.hpp>

class Script
{
public:
	Script(std::string filepath);
	~Script();

	void Compile(std::string filepath = "");

	void Execute();

	std::string GetFilepath() {return m_Filepath;}
private:
	sol::state m_Lua;
	sol::load_result m_CompiledScript;

	sol::protected_function m_Update;
	sol::protected_function m_Start;

	std::string m_Filepath;
};

