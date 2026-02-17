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

	m_LastCompilationTime = std::filesystem::last_write_time(filepath);

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

// GET VELOCITY FUNCTIONS
float GetVelocityX(Script* script)
{
	PhysicsComponent* physicsComponent = script->m_ECS->GetObjectComponent<PhysicsComponent>(script->GetGameObject());
	if (physicsComponent != nullptr)
		return physicsComponent->m_Velocity.x;
	return 0.0f;
}
float GetVelocityY(Script* script)
{
	PhysicsComponent* physicsComponent = script->m_ECS->GetObjectComponent<PhysicsComponent>(script->GetGameObject());
	if (physicsComponent != nullptr)
		return physicsComponent->m_Velocity.y;
	return 0.0f;
}
float GetVelocityZ(Script* script)
{
	PhysicsComponent* physicsComponent = script->m_ECS->GetObjectComponent<PhysicsComponent>(script->GetGameObject());
	if (physicsComponent != nullptr)
		return physicsComponent->m_Velocity.z;
	return 0.0f;
}

bool IsKeyDown(const char c)
{
	return (LunaWindow::m_FocusedWindow->GetKey(GLFW_KEY_A + (std::toupper(c) - 'A')) == GLFW_PRESS);
}
float GetPositionX(Script* script)
{
	Transform* transform = script->m_ECS->GetObjectComponent<Transform>(script->GetGameObject());
	return transform->position.x;
}


void Script::BindFunctions()
{

	m_Lua.set_function("Translate", [this](float x, float y, float z) {
		Transform* transform = this->m_ECS->GetObjectComponent<Transform>(this->m_GameObject);
		transform->position += glm::vec3(x, y, z);
	});
	m_Lua.set_function("SetPosition", [this](float x, float y, float z) {
		Transform* transform = this->m_ECS->GetObjectComponent<Transform>(this->m_GameObject);
		transform->position = glm::vec3(x, y, z);
	});

	m_Lua.set_function("AddForce", [this](float x, float y, float z) {
		PhysicsComponent* physicsComponent = this->m_ECS->GetObjectComponent<PhysicsComponent>(this->m_GameObject);
		std::cout << "trying something" << std::endl;
		if (physicsComponent != nullptr)
		{
			physicsComponent->m_NetForce += glm::vec3(x, y, z);
			std::cout << "and did something" << std::endl;
		}
		
		});

	m_Lua.set_function("SetVelocity", [this](float x, float y, float z) {
		PhysicsComponent* physicsComponent = this->m_ECS->GetObjectComponent<PhysicsComponent>(this->m_GameObject);
		if (physicsComponent != nullptr)
			physicsComponent->m_Velocity = glm::vec3(x, y, z);
		});

	m_Lua.set_function("SetForce", [this](float x, float y, float z) {
		PhysicsComponent* physicsComponent = this->m_ECS->GetObjectComponent<PhysicsComponent>(this->m_GameObject);
		if (physicsComponent != nullptr)
			physicsComponent->m_NetForce = glm::vec3(x, y, z);
		});

	m_Lua.set_function("SetAcceleration", [this](float x, float y, float z) {
		PhysicsComponent* physicsComponent = this->m_ECS->GetObjectComponent<PhysicsComponent>(this->m_GameObject);
		if (physicsComponent != nullptr)
			physicsComponent->m_Acceleration = glm::vec3(x, y, z);
		});

	// Bindings that pass a reference to this Script instance to the helper functions
	m_Lua.set_function("IsKeyDown", [this](const char c) -> bool { return IsKeyDown(c); }); 
	m_Lua.set_function("GetPositionX", [this]() -> float {return GetPositionX(this); });

	m_Lua.set_function("GetVelocityX", [this]() -> float { return GetVelocityX(this); });
	m_Lua.set_function("GetVelocityY", [this]() -> float { return GetVelocityY(this); });
	m_Lua.set_function("GetVelocityZ", [this]() -> float { return GetVelocityZ(this); });
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

bool Script::NeedsCompiling()
{
	return (std::filesystem::last_write_time(m_Filepath) > m_LastCompilationTime);
}
