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

		// Handle Collisions
		for (auto& [id, component] : colliderComponents)
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

					// Distance Check
					if(component.m_Shape == ColliderShape::Sphere && component2.m_Shape == ColliderShape::Sphere)
					{
						if (glm::length(transform2.position - transform1.position) <= component.m_ColliderSize.x + component2.m_ColliderSize.x)
						{
							std::cout << "Colliding" << std::endl;
						}
					}

					// OBB
					if (component.m_Shape == ColliderShape::Cube && component2.m_Shape == ColliderShape::Cube)
					{
						glm::mat3 r1 = glm::toMat3(transform1.rotation);
						glm::vec3 o1[3] = {r1 * glm::vec3(1, 0, 0), r1 * glm::vec3(0, 1, 0), r1 * glm::vec3(0, 0, 1) }; // orientation of the first collider
						glm::mat3 r2 = glm::toMat3(transform2.rotation);
						glm::vec3 o2[3] = {r2 * glm::vec3(1, 0, 0), r2 * glm::vec3(0, 1, 0), r2 * glm::vec3(0, 0, 1) }; // orientation of the ssecond collider

						glm::vec3 axes[15] = {
							o1[0], o1[1], o1[2],
							o2[0], o2[1], o2[2],

							glm::cross(o1[0], o2[0]), glm::cross(o1[0], o2[1]), glm::cross(o1[0], o2[2]),
							glm::cross(o1[1], o2[0]), glm::cross(o1[1], o2[1]), glm::cross(o1[1], o2[2]),
							glm::cross(o1[2], o2[0]), glm::cross(o1[2], o2[1]), glm::cross(o1[2], o2[2])

						};

						bool colliding = true;
						for (glm::vec3 v : axes)
						{
							// if length is tiny then skip
							if (glm::length2(v) < 1e-6f) continue;

							v = glm::normalize(v);

							// scary math
							float rA = (((component.m_ColliderSize.x * transform1.scale.x) / 2) * glm::abs(glm::dot(v, o1[0])) +
										((component.m_ColliderSize.y * transform1.scale.y) / 2) * glm::abs(glm::dot(v, o1[1])) +
										((component.m_ColliderSize.z * transform1.scale.z) / 2) * glm::abs(glm::dot(v, o1[2])));
							float rB = (((component2.m_ColliderSize.x * transform2.scale.x) / 2) * glm::abs(glm::dot(v, o2[0])) +
										((component2.m_ColliderSize.y * transform2.scale.y) / 2) * glm::abs(glm::dot(v, o2[1])) +
										((component2.m_ColliderSize.z * transform2.scale.z) / 2) * glm::abs(glm::dot(v, o2[2])));
							// slightly less scary maths
							float d = glm::abs(glm::dot(transform1.position - transform2.position, v));

							if (d > rA + rB)
							{
								colliding = false;
							}
						}
						if(colliding)
							std::cout << "COLLIDING" << std::endl;
						else
							std::cout << "NOT COLLIDING" << std::endl;
					}
				}
			}
			
		}

		// Simulate Physics
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
