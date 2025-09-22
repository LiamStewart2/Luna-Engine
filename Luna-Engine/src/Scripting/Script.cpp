#include "Script.h"

#include <cctype>
#include "../Core/LunaWindow.h"

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
		return;
	}

	BindFunctions();

	sol::protected_function_result result = m_CompiledScript();
	if (!result.valid())
	{
		sol::error err = result;
		std::cerr << "[SCRIPT]" << m_Filepath << " runtime error: " << err.what() << std::endl;
		return;
	}

	m_Start = m_Lua["Start"];
	m_Update = m_Lua["Update"];
}

void Script::BindFunctions()
{
	m_Lua.set_function("IsKeyDown", [this](const char c) {
		if(LunaWindow::m_FocusedWindow->GetKey(GLFW_KEY_A + (std::toupper(c) - 'A')) == GLFW_PRESS)
			return true;
		return false;

	});

	m_Lua.set_function("Translate", [this](float x, float y, float z) {
		Transform* transform = this->m_ECS->GetObjectComponent<Transform>(this->m_GameObject);
		transform->position += glm::vec3(x, y, z);
	});
}

void Script::Execute(unsigned int gameobject)
{
	m_GameObject = gameobject;
	if(!m_CompiledScript.valid())
		return;

	if (m_Update.valid())
	{
		sol::protected_function_result updateResult = m_Update();
		if (!updateResult.valid())
		{
			sol::error err = updateResult;
			std::cerr << "[SCRIPT] " << m_Filepath << " ERROR: " << err.what() << std::endl;
		}
	}
}