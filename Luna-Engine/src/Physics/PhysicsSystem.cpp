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

void Luna::PhysicsSystem::Collision_SphereSphere(EntityComponentSystem* ECS, unsigned int id, unsigned int id2, float deltaTime)
{
	PhysicsComponent* physics1 = ECS->GetObjectComponent<PhysicsComponent>(id);
	PhysicsComponent* physics2 = ECS->GetObjectComponent<PhysicsComponent>(id2);
	ColliderComponent* collider1 = ECS->GetObjectComponent<ColliderComponent>(id);
	ColliderComponent* collider2 = ECS->GetObjectComponent<ColliderComponent>(id2);
	Transform* transform1 = ECS->GetObjectComponent<Transform>(id);
	Transform* transform2 = ECS->GetObjectComponent<Transform>(id2);

	float radius1 = collider1->m_ColliderSize.x * transform1->scale.x;
	float radius2 = collider2->m_ColliderSize.x * transform2->scale.x;
	float distance = glm::length(transform1->position - transform2->position);

	if (distance < radius1 + radius2)
		CollisionResponse(ECS, id, id2, glm::normalize(transform1->position - transform2->position), (radius1 + radius2) - distance, deltaTime);
}

void Luna::PhysicsSystem::Collision_SphereRect(EntityComponentSystem* ECS, unsigned int id, unsigned int id2, float deltaTime)
{
	PhysicsComponent* physics1 = ECS->GetObjectComponent<PhysicsComponent>(id);
	PhysicsComponent* physics2 = ECS->GetObjectComponent<PhysicsComponent>(id2);
	ColliderComponent* collider1 = ECS->GetObjectComponent<ColliderComponent>(id);
	ColliderComponent* collider2 = ECS->GetObjectComponent<ColliderComponent>(id2);
	Transform* transform1 = ECS->GetObjectComponent<Transform>(id);
	Transform* transform2 = ECS->GetObjectComponent<Transform>(id2);


	bool sphereFirst = (collider1->m_Shape == ColliderShape::Sphere);
	// if the sphere is not the first object, swap the references for simplicity
	if(!sphereFirst)
	{
		PhysicsComponent* tempPhysics = physics2;
		ColliderComponent* tempCollider = collider2;
		Transform* tempTransform = transform2;

		physics2 = physics1;
		collider2 = collider1;
		transform2 = transform1;

		physics1 = tempPhysics;
		collider1 = tempCollider;
		transform1 = tempTransform;
	}

	float cx = transform1->position.x;
	float cy = transform1->position.y;
	float cz = transform1->position.z;
	float radius = collider1->m_ColliderSize.x * transform1->scale.x;

	float rx = transform2->position.x;
	float ry = transform2->position.y;
	float rz = transform2->position.z;
	float rw = collider2->m_ColliderSize.x * transform2->scale.x;
	float rh = collider2->m_ColliderSize.y * transform2->scale.y;
	float rd = collider2->m_ColliderSize.z * transform2->scale.z;

	float closestX = std::max(rx - rw / 2, std::min(cx, rx + rw / 2));
	float closestY = std::max(ry - rh / 2, std::min(cy, ry + rh / 2));
	float closestZ = std::max(rz - rd / 2, std::min(cz, rz + rd / 2));

	float distanceSquared = (closestX - cx) * (closestX - cx) +
							(closestY - cy) * (closestY - cy) +
							(closestZ - cz) * (closestZ - cz);
	
	bool colliding = distanceSquared < (radius * radius);

	if(colliding)
		std::cout << "Fortnite" << std::endl;

}

void Luna::PhysicsSystem::Collision_RectRect(EntityComponentSystem* ECS, unsigned int id, unsigned int id2, float deltaTime)
{
	PhysicsComponent* physics1 = ECS->GetObjectComponent<PhysicsComponent>(id);
	PhysicsComponent* physics2 = ECS->GetObjectComponent<PhysicsComponent>(id2);
	ColliderComponent* collider1 = ECS->GetObjectComponent<ColliderComponent>(id);
	ColliderComponent* collider2 = ECS->GetObjectComponent<ColliderComponent>(id2);
	Transform* transform1 = ECS->GetObjectComponent<Transform>(id);
	Transform* transform2 = ECS->GetObjectComponent<Transform>(id2);


	// Calculate all axis to check
	glm::mat3 r1 = glm::toMat3(transform1->rotation);
	glm::vec3 o1[3] = { r1 * glm::vec3(1, 0, 0), r1 * glm::vec3(0, 1, 0), r1 * glm::vec3(0, 0, 1) }; // orientation of the first collider
	glm::mat3 r2 = glm::toMat3(transform2->rotation);
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
		v = glm::normalize(v);

		// scary math
		float rA = (((collider1->m_ColliderSize.x * transform1->scale.x)) * glm::abs(glm::dot(v, o1[0])) +
			((collider1->m_ColliderSize.y * transform1->scale.y)) * glm::abs(glm::dot(v, o1[1])) +
			((collider1->m_ColliderSize.z * transform1->scale.z)) * glm::abs(glm::dot(v, o1[2])));
		float rB = (((collider2->m_ColliderSize.x * transform2->scale.x)) * glm::abs(glm::dot(v, o2[0])) +
			((collider2->m_ColliderSize.y * transform2->scale.y)) * glm::abs(glm::dot(v, o2[1])) +
			((collider2->m_ColliderSize.z * transform2->scale.z)) * glm::abs(glm::dot(v, o2[2])));
		// slightly less scary maths
		float d = glm::abs(glm::dot(transform1->position - transform2->position, v));

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
		std::cout << "Collision Detected: " << minPenetration << " units deep" << std::endl;
		CollisionResponse(ECS, id, id2, bestAxis, minPenetration, deltaTime);
	}
}

void Luna::PhysicsSystem::CollisionResponse(EntityComponentSystem* ECS, unsigned int id, unsigned int id2, glm::vec3 collisionNormal, float penetrationDepth, float deltaTime)
{
	PhysicsComponent* physics1 = ECS->GetObjectComponent<PhysicsComponent>(id);
	PhysicsComponent* physics2 = ECS->GetObjectComponent<PhysicsComponent>(id2);
	ColliderComponent* collider1 = ECS->GetObjectComponent<ColliderComponent>(id);
	ColliderComponent* collider2 = ECS->GetObjectComponent<ColliderComponent>(id2);
	Transform* transform1 = ECS->GetObjectComponent<Transform>(id);
	Transform* transform2 = ECS->GetObjectComponent<Transform>(id2);

	// calculate the inverse mass of both objects, if an object is dynamic, use 0.0f as the mass
	float invMassA = (physics1->m_Dynamic) ? 1.0f / physics1->m_Mass : 0.0f;
	float invMassB = (physics2->m_Dynamic) ? 1.0f / physics2->m_Mass : 0.0f;

	// if both objects are static or have no mass then ignore the collision
	if (invMassA + invMassB == 0.0f)
		return;

	// make sure that the axis is facing FROM the other collider towards the primary collider
	if (glm::dot(collisionNormal, transform1->position - transform2->position) < 0.0f)
	{
		collisionNormal = -collisionNormal;
	}

	// calculate the relative velocity of the objects
	glm::vec3 relativeVelocity = physics1->m_Velocity - physics2->m_Velocity;

	// figure out the direction of the relative velocities
	float vn = dot(relativeVelocity, collisionNormal);
	if (vn > 0.0f) return; // separating

	// if hitting a floor, have a lower restitution to avoid infinite bouncing
	float restitution = (physics1->m_Restitution + physics2->m_Restitution) / 2; // Coefficient of restitution (bounciness)


	float j = -(1.0f + restitution) * vn;
	j /= (invMassA + invMassB);

	std::cout << "Impulse: " << j << std::endl;
	if (physics1->m_Dynamic)
		physics1->m_Velocity += j * invMassA * collisionNormal;
	if (physics2->m_Dynamic)
		physics2->m_Velocity -= j * invMassB * collisionNormal;


	// Calculate correction to resolve penetration
	glm::vec3 correction = collisionNormal * std::max(penetrationDepth, 0.0f);

	if (physics1->m_Dynamic)
		transform1->position += correction  ;
	if (physics2->m_Dynamic)
		transform2->position -= correction  ;
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

			if (id <= id2)
				continue;

			std::cout << "ATTEMPT: " << names[id].m_Name << ", " << names[id2].m_Name << std::endl;

			// SPHERE ON SPHERE
			if (component.m_Shape == ColliderShape::Sphere && component2.m_Shape == ColliderShape::Sphere)
				Collision_SphereSphere(ECS, id, id2, deltaTime);

			// SPHERE ON OBB
			else if (component.m_Shape == ColliderShape::Sphere || component2.m_Shape == ColliderShape::Sphere)
				Collision_SphereRect(ECS, id, id2, deltaTime);
		
			// OBB
			else if (component.m_Shape == ColliderShape::Cube && component2.m_Shape == ColliderShape::Cube)
				Collision_RectRect(ECS, id, id2, deltaTime);
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

			const float angularDamping = 0.98f; // try 0.95–0.99
			component.m_AngularVelocity *= angularDamping;

			glm::mat3 R = glm::toMat3(transforms[id].rotation);
			glm::mat3 I_inv_world = R * component.m_InverseTensor * glm::transpose(R);

			component.m_AngularAcceleration += I_inv_world * component.m_NetTorque;

			component.m_AngularVelocity += component.m_AngularAcceleration * deltaTime;
		}
	}
}
