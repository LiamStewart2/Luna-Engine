#pragma once

#include <unordered_map>

#include "GLM/gtx/quaternion.hpp"
#include "GLM/vec3.hpp"
#include "../Core/Timer.h"
#include "../ECS/ECS.h"

namespace Luna
{
	class PhysicsSystem
	{
	public:
		PhysicsSystem();
		~PhysicsSystem();

		void Update(EntityComponentSystem* ECS, bool runtime);
		void Init(EntityComponentSystem* ECS);
	private:

		void HandleCollisions(EntityComponentSystem* ECS, float deltaTime);

		void HandlePhysics(EntityComponentSystem* ECS, float deltaTime);

		void UpdatePositions(EntityComponentSystem* ECS, float deltaTime);

		void InitTensors(EntityComponentSystem* ECS);

		void ApplyBoxTensor(PhysicsComponent& component, Transform& transform);


		void Collision_SphereSphere(EntityComponentSystem* ECS, unsigned int id, unsigned int id2, float deltaTime);
		void Collision_SphereRect(EntityComponentSystem* ECS, unsigned int id, unsigned int id2, float deltaTime);
		void Collision_RectRect(EntityComponentSystem* ECS, unsigned int id, unsigned int id2, float deltaTime);

		void CollisionResponse(EntityComponentSystem* ECS, unsigned int id, unsigned int id2, glm::vec3 collisionNormal, float penetrationDepth, float deltaTime);


		Timer timer;
	};
}

