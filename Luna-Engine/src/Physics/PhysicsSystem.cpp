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
		if (component.m_Simulate && component.m_BeingManipulated == false && component.m_Dynamic)
		{
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
			if(id <= id2)
				continue;

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

				float minPenetration = FLT_MAX;
				glm::vec3 bestAxis;

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

					float penetration = (rA + rB) - d;
					if (penetration < 0.0f)
					{
						colliding = false;
						break;
					}

					if (penetration < minPenetration)
					{
						minPenetration = penetration;
						bestAxis = v;
					}
				}
				if (colliding)
				{
					// Resolve Collision
					try
					{
						// Get the two physics colliders, if either object doesnt have a collider, an error will be thrown and the collision will be ignored
						PhysicsComponent& physicsComponent = physicsComponents.at(id);
						PhysicsComponent& physicsComponent2 = physicsComponents.at(id2);

						// calculate the inverse mass of both objects, if an object is dynamic, use 0.0f as the mass
						float invMassA = (physicsComponent.m_Dynamic) ? 1.0f / physicsComponent.m_Mass : 0.0f;
						float invMassB = (physicsComponent2.m_Dynamic) ? 1.0f / physicsComponent2.m_Mass : 0.0f;

						// if both objects are static or have no mass then ignore the collision
						if (invMassA + invMassB == 0.0f)
							continue;

						// make sure that the axis is facing FROM the other collider towards the primary collider
						if (dot(bestAxis, transform1.position - transform2.position) < 0.0f)
							bestAxis = -bestAxis;

						glm::vec3 collisionNormal = bestAxis;
						float penetrationDepth = minPenetration;

						// calculate the relative velocity of the objects
						glm::vec3 relativeVelocity = physicsComponent.m_Velocity - physicsComponent2.m_Velocity;

						// figure out the direction of the relative velocities
						float vn = dot(relativeVelocity, collisionNormal);
						if (vn > 0.0f) continue; // separating

						// if hitting a floor, have a lower restitution to avoid infinite bouncing
						float restitution = (collisionNormal.y > 0.7f) ? 0.0f : 0.2f; // Coefficient of restitution (bounciness)


						float j = -(1.0f + restitution) * vn;
						j /= (invMassA + invMassB);


						if(physicsComponent.m_Dynamic)
							physicsComponent.m_Velocity += j * invMassA * collisionNormal;
						if(physicsComponent2.m_Dynamic)
							physicsComponent2.m_Velocity -= j * invMassB * collisionNormal;


						// smoothish correction of overlapping 
						const float percent = 0.8f;   // push out most of penetration
						const float slop = 0.001f;    // allow tiny overlap

						glm::vec3 correction =
							collisionNormal * std::max(penetrationDepth - slop, 0.0f) /
							(invMassA + invMassB) * percent;

						if(physicsComponent.m_Dynamic)
							transforms[id].position += correction * invMassA;
						if (physicsComponent2.m_Dynamic)
							transforms[id2].position -= correction * invMassB;
					}
					
					catch (const std::out_of_range& e)
					{
						std::cerr << "All colliders need a physics component (use a dynamic collider type)" << std::endl;
						
					}
				}
			}
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
		if (component.m_Simulate && component.m_BeingManipulated == false && component.m_Dynamic)
		{
			// Gravity
			component.m_NetForce -= glm::vec3(0, component.m_GravityValue * component.m_Mass, 0);

			//Drag  Force = 0.5 * density of atmosphere * drag coefficient * reference area (scale)
			float atmosphereDensity = 0.07f;
			float dragCoefficient = 1.05f;
			component.m_NetForce += 0.5f * atmosphereDensity * dragCoefficient * (transforms[id].scale * transforms[id].scale) * (glm::length(component.m_Velocity) * component.m_Velocity);

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
