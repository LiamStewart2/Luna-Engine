#include "PhysicsSystem.h"

Luna::PhysicsSystem::PhysicsSystem()
{
	timer = Timer();
}

Luna::PhysicsSystem::~PhysicsSystem()
{

}

void Luna::PhysicsSystem::Update(EntityComponentSystem* ECS, bool runtime)
{
	if(runtime)
	{
		std::unordered_map<unsigned int, PhysicsComponent>& physicsComponents = ECS->GetAllComponentsOfType<PhysicsComponent>();
		std::unordered_map<unsigned int, Transform>& transforms = ECS->GetAllComponentsOfType<Transform>();

		for (auto& [id, component] : physicsComponents)
		{
			if (component.m_Simulate && component.m_BeingManipulated == false)
			{

				if (transforms[id].position.y > 0)
				{

					component.m_Velocity.y -= (component.m_GravityValue * component.m_Mass); //* (float)timer.DeltaTime();
					transforms[id].position += component.m_Velocity * (float)timer.DeltaTime();
				}
				else
				{
					component.m_Velocity.y = 0;
				}
			}
			else
			{
				component.m_Velocity.y = 0;
			}
		}
	}
	timer.Tick();
}
