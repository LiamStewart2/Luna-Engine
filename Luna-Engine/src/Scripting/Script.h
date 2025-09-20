#pragma once

#include <SOL/sol.hpp>

class Script
{
public:
	Script(std::string filepath);
	~Script();

	void Compile(std::string filepath = "");

	void Execute();

private:
	sol::state m_Lua;
	sol::load_result m_CompiledScript;

	std::string m_Filepath;
};

