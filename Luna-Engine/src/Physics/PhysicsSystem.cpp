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

		std::cout << "Physics System Update - Delta Time: " << deltaTime << " seconds" << std::endl;

		HandlePhysics(ECS, deltaTime);
		HandleCollisions(ECS, deltaTime);

		UpdatePositions(ECS, deltaTime);
	}
	timer.Tick();
}

void Luna::PhysicsSystem::UpdatePositions(EntityComponentSystem* ECS, float deltaTime)
{
	std::unordered_map<unsigned int, PhysicsComponent>& physicsComponents = ECS->GetAllComponentsOfType<PhysicsComponent>();
	std::unordered_map<unsigned int, Transform>& transforms = ECS->GetAllComponentsOfType<Transform>();
	// Update Positions
	for (auto& [id, component] : physicsComponents)
	{
		if (component.m_Simulate && component.m_BeingManipulated == false)
		{
			glm::vec3 deltaVelocity = component.m_Velocity;
			glm::vec3 velocityChange = glm::vec3(0);
			

			transforms[id].position += component.m_Velocity * deltaTime;

			component.m_NetForce = glm::vec3(0, 0, 0);
			component.m_Acceleration = glm::vec3(0, 0, 0);
		}
	}
}

void Luna::PhysicsSystem::HandleCollisions(EntityComponentSystem* ECS, float deltaTime)
{
	std::unordered_map<unsigned int, PhysicsComponent>& physicsComponents = ECS->GetAllComponentsOfType<PhysicsComponent>();
	std::unordered_map<unsigned int, ColliderComponent>& colliderComponents = ECS->GetAllComponentsOfType<ColliderComponent>();
	std::unordered_map<unsigned int, Transform>& transforms = ECS->GetAllComponentsOfType<Transform>();

	// Handle Collisions
	for (auto& [id, component] : colliderComponents)
	{
		for (auto& [id2, component2] : colliderComponents)
		{
			if (id != id2)
			{

				Transform transform1;
				Transform transform2;
				glm::vec3 tempSkew;
				glm::vec4 tempPerspective;

				glm::decompose(transforms[id].transformMatrix, transform1.scale, transform1.rotation, transform1.position, tempSkew, tempPerspective);
				glm::decompose(transforms[id2].transformMatrix, transform2.scale, transform2.rotation, transform2.position, tempSkew, tempPerspective);

				// Distance Check
				if (component.m_Shape == ColliderShape::Sphere && component2.m_Shape == ColliderShape::Sphere)
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
					glm::vec3 o1[3] = { r1 * glm::vec3(1, 0, 0), r1 * glm::vec3(0, 1, 0), r1 * glm::vec3(0, 0, 1) }; // orientation of the first collider
					glm::mat3 r2 = glm::toMat3(transform2.rotation);
					glm::vec3 o2[3] = { r2 * glm::vec3(1, 0, 0), r2 * glm::vec3(0, 1, 0), r2 * glm::vec3(0, 0, 1) }; // orientation of the ssecond collider

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
						float rA = (((component.m_ColliderSize.x * transform1.scale.x)) * glm::abs(glm::dot(v, o1[0])) +
							((component.m_ColliderSize.y * transform1.scale.y)) * glm::abs(glm::dot(v, o1[1])) +
							((component.m_ColliderSize.z * transform1.scale.z)) * glm::abs(glm::dot(v, o1[2])));
						float rB = (((component2.m_ColliderSize.x * transform2.scale.x)) * glm::abs(glm::dot(v, o2[0])) +
							((component2.m_ColliderSize.y * transform2.scale.y)) * glm::abs(glm::dot(v, o2[1])) +
							((component2.m_ColliderSize.z * transform2.scale.z)) * glm::abs(glm::dot(v, o2[2])));
						// slightly less scary maths
						float d = glm::abs(glm::dot(transform1.position - transform2.position, v));

						if (d > rA + rB)
						{
							colliding = false;
						}
					}
					if (colliding)
					{
						// Resolve Collision
						std::cout << "COLLIDING" << std::endl;
						try
						{
							PhysicsComponent& physicsComponent = physicsComponents.at(id);

							if (physicsComponent.m_Simulate && physicsComponent.m_BeingManipulated == false)
							{

								glm::vec3 collisionNormal = glm::normalize(transforms[id].position - transforms[id2].position);
								float restitution = 0.4f; // Coefficient of restitution (bounciness)

								glm::vec3 otherVelocity = glm::vec3(0.0f);
								float otherMass = 100.0f;
								try
								{
									PhysicsComponent& physicsComponent2 = physicsComponents.at(id2);
									otherVelocity = physicsComponent2.m_Velocity;
									otherMass = physicsComponent2.m_Mass;
								}
								catch (const std::out_of_range& e)
								{
									otherVelocity = glm::vec3(0.0f);
									otherMass = 100.0f;
								}

								glm::vec3 relativeVelocity = physicsComponent.m_Velocity - otherVelocity;
								glm::vec3 vj = collisionNormal * relativeVelocity * -10.0f;

								glm::vec3 j = vj / (1 / physicsComponent.m_Mass + 1 / otherMass);
								std::cout << "Impulse: " << j.x << ", " << j.y << ", " << j.z << std::endl;
								component.m_HandledVelocity = 1 / physicsComponent.m_Mass * j * restitution * collisionNormal;
								std::cout << "Handled: " << component.m_HandledVelocity.x << ", " << component.m_HandledVelocity.y << ", " << component.m_HandledVelocity.z << std::endl;

							}
						}
						catch (const std::out_of_range& e)
						{
							continue;
						}
					}
					else
						std::cout << "NOT COLLIDING" << std::endl;
				}
			}
		}
	}
	UpdateCollidedPhysics(ECS, deltaTime);
}

void Luna::PhysicsSystem::UpdateCollidedPhysics(EntityComponentSystem* ECS, float deltaTime)
{
	std::unordered_map<unsigned int, PhysicsComponent>& physicsComponents = ECS->GetAllComponentsOfType<PhysicsComponent>();
	std::unordered_map<unsigned int, ColliderComponent>& colliderComponents = ECS->GetAllComponentsOfType<ColliderComponent>();
	std::unordered_map<unsigned int, Transform>& transforms = ECS->GetAllComponentsOfType<Transform>();
	for (auto& [id, component] : colliderComponents)
	{
		try
		{
			PhysicsComponent& physicsComponent = physicsComponents.at(id);
			if (physicsComponent.m_Simulate && physicsComponent.m_BeingManipulated == false)
			{
				physicsComponent.m_Velocity += component.m_HandledVelocity;
				component.m_HandledVelocity = glm::vec3(0);
			}
		}
		catch (const std::out_of_range& e)
		{
			continue;
		}
	}
}

void Luna::PhysicsSystem::HandlePhysics(EntityComponentSystem* ECS, float deltaTime)
{
	std::unordered_map<unsigned int, PhysicsComponent>& physicsComponents = ECS->GetAllComponentsOfType<PhysicsComponent>();
	std::unordered_map<unsigned int, ColliderComponent>& colliderComponents = ECS->GetAllComponentsOfType<ColliderComponent>();
	std::unordered_map<unsigned int, Transform>& transforms = ECS->GetAllComponentsOfType<Transform>();
	// Simulate Physics
	for (auto& [id, component] : physicsComponents)
	{
		if (component.m_Simulate && component.m_BeingManipulated == false)
		{
			// Gravity
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
		}
	}
}
