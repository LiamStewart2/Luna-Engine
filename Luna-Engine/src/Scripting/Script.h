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

	std::string m_Filepath;
};

