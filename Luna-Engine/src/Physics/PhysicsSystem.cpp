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
		std::unordered_map<unsigned int, ColliderComponent>& colliderComponents = ECS->GetAllComponentsOfType<ColliderComponent>();
		std::unordered_map<unsigned int, Transform>& transforms = ECS->GetAllComponentsOfType<Transform>();
		float deltaTime = (float)timer.DeltaTime();

		for (auto& [id, component] : colliderComponents)
		{
			if (component.m_Shape == ColliderShape::Sphere)
			{
				for (auto& [id2, component2] : colliderComponents)
				{
					if(id != id2)
					{

						Transform transform1;
						Transform transform2;
						glm::vec3 tempSkew;
						glm::vec4 tempPerspective;

						glm::decompose(transforms[id].transformMatrix, transform1.scale, transform1.rotation, transform1.position, tempSkew, tempPerspective);
						glm::decompose(transforms[id2].transformMatrix, transform2.scale, transform2.rotation, transform2.position, tempSkew, tempPerspective);


						if (glm::length(transform2.position - transform1.position) <= component.m_ColliderSize.x + component2.m_ColliderSize.x)
						{
							std::cout << "Colliding" << std::endl;
						}
					}
				}
			}
		}

		for (auto& [id, component] : physicsComponents)
		{
			if (component.m_Simulate && component.m_BeingManipulated == false)
			{
				// Gravity
				if (transforms[id].position.y > 0)
					component.m_NetForce -= glm::vec3(0, component.m_GravityValue * component.m_Mass, 0);

				//Drag  Force = 0.5 * density of atmosphere * drag coefficient * reference area (scale)
				float atmosphereDensity = 0.07f;
				float dragCoefficient = 1.05f;
				component.m_NetForce += 0.5f * atmosphereDensity * dragCoefficient * (transforms[id].scale * transforms[id].scale) * (component.m_Velocity * component.m_Velocity);
				
				// Friction

				if (transforms[id].position.y <= 0) 
				{
					glm::vec3 frictionForce = -component.m_Velocity;

					component.m_NetForce.x += 0.9f * frictionForce.x;
				}


				component.m_Acceleration += component.m_NetForce / component.m_Mass;

				component.m_Velocity += component.m_Acceleration * deltaTime;

				transforms[id].position += component.m_Velocity * deltaTime;

				if (transforms[id].position.y <= 0)
				{
					transforms[id].position.y = 0;
					component.m_Velocity.y = 0;
				}

				component.m_NetForce = glm::vec3(0, 0, 0);
				component.m_Acceleration = glm::vec3(0, 0, 0);
			}
		}
	}
	timer.Tick();
}
