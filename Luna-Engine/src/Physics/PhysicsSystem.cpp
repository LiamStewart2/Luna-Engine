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

void Luna::PhysicsSystem::Init(EntityComponentSystem* ECS)
{
	InitTensors(ECS);
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

			glm::quat omega = glm::quat(0.0f, component.m_AngularVelocity);
			glm::quat dq = 0.5f * omega * transforms[id].rotation;
			transforms[id].rotation += dq * deltaTime;
			transforms[id].rotation = glm::normalize(transforms[id].rotation); // normalize the rotation to avoid non 1 quaternion lengths
		}
	}
}

void Luna::PhysicsSystem::InitTensors(EntityComponentSystem* ECS)
{
	std::unordered_map<unsigned int, PhysicsComponent>& physicsComponents = ECS->GetAllComponentsOfType<PhysicsComponent>();
	std::unordered_map<unsigned int, Transform>& transforms = ECS->GetAllComponentsOfType<Transform>();
	for (auto& [id, component] : physicsComponents)
	{
		ApplyBoxTensor(component, transforms[id]);
	}
}

void Luna::PhysicsSystem::ApplyBoxTensor(PhysicsComponent& component, Transform& transform)
{

	if(component.m_Dynamic)
	{
		float hx = transform.scale.x;
		float hy = transform.scale.y;
		float hz = transform.scale.z;

		float x2 = 4.0f * hx * hx;
		float y2 = 4.0f * hy * hy;
		float z2 = 4.0f * hz * hz;

		float Ixx = (1.0f / 12.0f) * component.m_Mass * (y2 + z2);
		float Iyy = (1.0f / 12.0f) * component.m_Mass * (x2 + z2);
		float Izz = (1.0f / 12.0f) * component.m_Mass * (x2 + y2);

		component.m_InertiaTensor = glm::mat3(
			Ixx, 0, 0,
			0, Iyy, 0,
			0, 0, Izz
		);

		component.m_InverseTensor = glm::inverse(component.m_InertiaTensor); // inverse of the world space tensor
	}
	else
	{
		component.m_InertiaTensor = glm::mat3(0);
		component.m_InverseTensor = glm::mat3(0);
	}
}

void Luna::PhysicsSystem::HandleCollisions(EntityComponentSystem* ECS, float deltaTime)
{
	std::unordered_map<unsigned int, PhysicsComponent>& physicsComponents = ECS->GetAllComponentsOfType<PhysicsComponent>();
	std::unordered_map<unsigned int, ColliderComponent>& colliderComponents = ECS->GetAllComponentsOfType<ColliderComponent>();
	std::unordered_map<unsigned int, Transform>& transforms = ECS->GetAllComponentsOfType<Transform>();
	std::unordered_map<unsigned int, NameComponent>& names = ECS->GetAllComponentsOfType<NameComponent>();

	// Handle Collisions
	for (auto& [id, component] : colliderComponents)
	{
		for (auto& [id2, component2] : colliderComponents)
		{

			if(id <= id2)
				continue;

			std::cout << "ATTEMPT: " << names[id].m_Name << ", " << names[id2].m_Name << std::endl;

			Transform transform1;
			Transform transform2;

			glm::vec3 tempSkew;
			glm::vec4 tempPerspective;

			glm::decompose(transforms[id].transformMatrix, transform1.scale, transform1.rotation, transform1.position, tempSkew, tempPerspective);
			glm::decompose(transforms[id2].transformMatrix, transform2.scale, transform2.rotation, transform2.position, tempSkew, tempPerspective);

			// OBB
			if (component.m_Shape == ColliderShape::Cube && component2.m_Shape == ColliderShape::Cube)
			{
				glm::mat3 r1 = glm::toMat3(transform1.rotation);
				glm::vec3 o1[3] = { r1 * glm::vec3(1, 0, 0), r1 * glm::vec3(0, 1, 0), r1 * glm::vec3(0, 0, 1) }; // orientation of the first collider
				glm::mat3 r2 = glm::toMat3(transform2.rotation);
				glm::vec3 o2[3] = { r2 * glm::vec3(1, 0, 0), r2 * glm::vec3(0, 1, 0), r2 * glm::vec3(0, 0, 1) }; // orientation of the ssecond collider

				glm::vec3 axis[15] = {
					o1[0], o1[1], o1[2],
					o2[0], o2[1], o2[2],

					glm::cross(o1[0], o2[0]), glm::cross(o1[0], o2[1]), glm::cross(o1[0], o2[2]),
					glm::cross(o1[1], o2[0]), glm::cross(o1[1], o2[1]), glm::cross(o1[1], o2[2]),
					glm::cross(o1[2], o2[0]), glm::cross(o1[2], o2[1]), glm::cross(o1[2], o2[2])

				};

				float minPenetration = FLT_MAX;
				glm::vec3 bestAxis;

				bool colliding = true;
				for (glm::vec3 v : axis)
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
						float restitution = (collisionNormal.y > 0.7f) ? 0.1f : 0.4f; // Coefficient of restitution (bounciness)

						if (abs(vn) < 0.05f)
							restitution = 0.0f;

						// smoothish correction of overlapping 
						const float percent = 0.8f;   // push out most of penetration
						const float slop = 0.001f;    // allow tiny overlap

						glm::vec3 correction =
							collisionNormal * std::max(penetrationDepth - slop, 0.0f) /
							(invMassA + invMassB) * percent;

						if (physicsComponent.m_Dynamic)
							transforms[id].position += correction * invMassA;
						if (physicsComponent2.m_Dynamic)
							transforms[id2].position -= correction * invMassB;
						

						// Resolve Collision Impulses
						glm::vec3 contactPoint = transform1.position + collisionNormal * (component.m_ColliderSize.x * transform1.scale.x);

						glm::vec3 halfExtents = component.m_ColliderSize * transform1.scale;
						glm::vec3 localContact = glm::inverse(glm::toMat3(transform1.rotation)) *
							(contactPoint - transform1.position);
						localContact = glm::clamp(localContact, -halfExtents, halfExtents);
						contactPoint = transform1.position +
							glm::toMat3(transform1.rotation) * localContact;
						// relative contact points

						glm::vec3 relativeContactPointA = contactPoint - transform1.position;
						glm::vec3 relativeContactPointB = contactPoint - transform2.position;

						// World space inverse tensors
						glm::mat3 R1 = glm::toMat3(transform1.rotation);
						glm::mat3 R2 = glm::toMat3(transform2.rotation);
						glm::mat3 I_inv_worldA = R1 * physicsComponent.m_InverseTensor * glm::transpose(R1);
						glm::mat3 I_inv_worldB = R2 * physicsComponent2.m_InverseTensor * glm::transpose(R2);


						glm::vec3 vel1 = physicsComponent.m_Velocity + glm::cross(physicsComponent.m_AngularVelocity, relativeContactPointA);
						glm::vec3 vel2 = physicsComponent2.m_Velocity + glm::cross(physicsComponent2.m_AngularVelocity, relativeContactPointB);
						glm::vec3 relativeVelocityAtContact = vel1 - vel2;
						float vnAtContact = dot(relativeVelocityAtContact, collisionNormal);

						// BIG FAT EQUATION TO FIND THE DIFFERENCE IN VELOCITY NEEDED;
						float denom = 
							(invMassA + invMassB) +
							glm::dot(collisionNormal,
								glm::cross(I_inv_worldA * glm::cross(relativeContactPointA, collisionNormal), relativeContactPointA) +
								glm::cross(I_inv_worldB * glm::cross(relativeContactPointB, collisionNormal), relativeContactPointB));


						float j = -(1.0f + restitution) * vnAtContact / denom;

						const float impulseEps = 1e-4f;
						if (glm::abs(j) < impulseEps)
							 continue;

						// Apply Collision Response
						if (physicsComponent.m_Dynamic)
						{
							physicsComponent.m_Velocity += (j * collisionNormal) * invMassA;
							physicsComponent.m_AngularVelocity += I_inv_worldA * glm::cross(relativeContactPointA, (j * collisionNormal));
							std::cout << "Entity: " << id << " Angular Vel: " << physicsComponent.m_AngularVelocity.x << ", " << physicsComponent.m_AngularVelocity.y << std::endl;
						}

						if (physicsComponent2.m_Dynamic)
						{
							physicsComponent2.m_Velocity -= (j * collisionNormal) * invMassB;
							physicsComponent2.m_AngularVelocity -= I_inv_worldB * glm::cross(relativeContactPointB, (j * collisionNormal));
							std::cout << "Entity: " << id2 << " Angular Vel: " << physicsComponent2.m_AngularVelocity.x << ", " << physicsComponent2.m_AngularVelocity.y << std::endl;
						}
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
			component.m_NetForce = glm::vec3(0, 0, 0);
			component.m_Acceleration = glm::vec3(0, 0, 0);
			component.m_NetTorque = glm::vec3(0, 0, 0);
			component.m_AngularAcceleration = glm::vec3(0, 0, 0);


			// Handle Velocity
			// Gravity
			component.m_NetForce -= glm::vec3(0, component.m_GravityValue * component.m_Mass, 0);

			//Drag  Force = 0.5 * density of atmosphere * drag coefficient * reference area (scale) * magnitude(velocity) * velocity
			float atmosphereDensity = 0.07f;
			float dragCoefficient = 1.05f;
			component.m_NetForce += 0.5f * atmosphereDensity * dragCoefficient * (transforms[id].scale * transforms[id].scale) * (glm::length(component.m_Velocity) * component.m_Velocity);


			component.m_Acceleration += component.m_NetForce / component.m_Mass;

			component.m_Velocity += component.m_Acceleration * deltaTime;

			// Handle Angular Velocity

			const float angularDamping = 0.99f; // try 0.95–0.99
			component.m_AngularVelocity *= angularDamping;

			glm::mat3 R = glm::toMat3(transforms[id].rotation);
			glm::mat3 I_inv_world = R * component.m_InverseTensor * glm::transpose(R);

			component.m_AngularAcceleration += I_inv_world * component.m_NetTorque;

			component.m_AngularVelocity += component.m_AngularAcceleration * deltaTime;
		}
	}
}
