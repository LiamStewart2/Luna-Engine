#include "PhysicsSystem.h"

Luna::PhysicsSystem::PhysicsSystem()
{

}

Luna::PhysicsSystem::~PhysicsSystem()
{

}

void Luna::PhysicsSystem::Update(EntityComponentSystem* ECS)
{
	std::unordered_map<unsigned int, PhysicsComponent>& physicsComponents = ECS->GetAllComponentsOfType<PhysicsComponent>();
	std::unordered_map<unsigned int, Transform>& transforms = ECS->GetAllComponentsOfType<Transform>();

	for (auto& [id, component] : physicsComponents)
	{
		if (component.m_Simulate)
		{

			if(transforms[id].position.y > 0)
			{
				component.m_Velocity.y -= component.m_GravityValue * component.m_Mass;
				
				transforms[id].position += component.m_Velocity;
			}
			else
			{
				component.m_Velocity.y = 0;
			}
		}
	}
	
}
