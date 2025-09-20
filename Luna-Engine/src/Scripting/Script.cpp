#include "Script.h"

Script::Script(std::string filepath)
{
	Compile(filepath);
}

Script::~Script()
{
}

void Script::Compile(std::string filepath)
{
	if(filepath == "") filepath = m_Filepath;
	else m_Filepath = filepath;

	m_Lua.open_libraries(sol::lib::base, sol::lib::math);

	m_CompiledScript = m_Lua.load_file(filepath);

	if (!m_CompiledScript.valid()) 
	{
		sol::error err = m_CompiledScript;
		std::cerr << "[SCRIPT] " << m_Filepath << " filepath failed to compile: " <<err.what() << std::endl;
	}
}

void Script::Execute()
{
	if(!m_CompiledScript.valid())
		return;

	sol::protected_function_result result = m_CompiledScript();
	if (!result.valid()) 
	{
		sol::error err = result;
		std::cerr << "[SCRIPT] " << m_Filepath << " filepath failed to compile: " << err.what() << std::endl;
		return;
	}


	sol::protected_function f = m_Lua["Update"];
	if (f.valid()) 
	{
		sol::protected_function_result updateResult = f();
		if (!updateResult.valid())
		{
			sol::error err = updateResult;
			std::cerr << "[SCRIPT] " << m_Filepath << " ERROR: " << err.what() << std::endl;
		}
	}
}
